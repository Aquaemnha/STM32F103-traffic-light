#include "fsm_automatic.h"
#include "global.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "button.h"

// Chạy chu kỳ 4 pha đèn nền khi hệ thống đang ở chế độ Tuning
static void run_auto_background_cycle(void) {
    switch (auto_bg_state) {
        case AUTO_RED1_GREEN2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);
            if (timer_flag[0] == 1) {
                auto_bg_state = AUTO_RED1_AMBER2;
                setTimer(0, time_amber);
            }
            break;

        case AUTO_RED1_AMBER2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_AMBER);
            if (timer_flag[0] == 1) {
                auto_bg_state = AUTO_GREEN1_RED2;
                setTimer(0, time_green);
            }
            break;

        case AUTO_GREEN1_RED2:
            traffic_set(1, LIGHT_GREEN);
            traffic_set(2, LIGHT_RED);
            if (timer_flag[0] == 1) {
                auto_bg_state = AUTO_AMBER1_RED2;
                setTimer(0, time_amber);
            }
            break;

        case AUTO_AMBER1_RED2:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_RED);
            if (timer_flag[0] == 1) {
                auto_bg_state = AUTO_RED1_GREEN2;
                setTimer(0, time_green);
            }
            break;

        default:
            auto_bg_state = AUTO_RED1_GREEN2;
            setTimer(0, time_green);
            break;
    }
}

void fsm_automatic_run(void) {
    // Nếu đang ở chế độ Tuning, đèn vẫn chạy chu kỳ Auto nền với thời gian cũ
    if (status == TUN_GREEN || status == TUN_AMBER) {
        run_auto_background_cycle();
        return;
    }

    switch (status) {
        case INIT:
            status = AUTO_RED1_GREEN2;
            auto_bg_state = AUTO_RED1_GREEN2;
            setTimer(0, time_green);
            break;

        case AUTO_RED1_GREEN2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);
            if (timer_flag[0] == 1) {
                status = AUTO_RED1_AMBER2;
                setTimer(0, time_amber);
            }
            // Đang pha Xanh -> chuyển thẳng sang Manual giữ nguyên pha
            if (isButtonPressed(0)) {
                status = MAN_RED1_GREEN2;
            }
            break;

        case AUTO_RED1_AMBER2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_AMBER);
            if (timer_flag[0] == 1) {
                status = AUTO_GREEN1_RED2;
                setTimer(0, time_green);
            }
            // Đang pha Vàng -> bật Vàng 2 tuyến trong 2s trước khi vào Manual
            if (isButtonPressed(0)) {
                status = MAN_WAIT_TO_GREEN1_RED2;
                setTimer(0, 2000);
            }
            break;

        case AUTO_GREEN1_RED2:
            traffic_set(1, LIGHT_GREEN);
            traffic_set(2, LIGHT_RED);
            if (timer_flag[0] == 1) {
                status = AUTO_AMBER1_RED2;
                setTimer(0, time_amber);
            }
            // Đang pha Xanh -> chuyển thẳng sang Manual giữ nguyên pha
            if (isButtonPressed(0)) {
                status = MAN_GREEN1_RED2;
            }
            break;

        case AUTO_AMBER1_RED2:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_RED);
            if (timer_flag[0] == 1) {
                status = AUTO_RED1_GREEN2;
                setTimer(0, time_green);
            }
            // Đang pha Vàng -> bật Vàng 2 tuyến trong 2s trước khi vào Manual
            if (isButtonPressed(0)) {
                status = MAN_WAIT_TO_RED1_GREEN2;
                setTimer(0, 2000);
            }
            break;

        default:
            break;
    }
}
