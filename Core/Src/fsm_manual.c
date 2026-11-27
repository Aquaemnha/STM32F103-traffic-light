#include "fsm_manual.h"
#include "fsm_tuning.h"
#include "global.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "button.h"

static void discard_all_buttons(void) {
    for (uint8_t i = 0; i < 4; i++) {
        (void)isButtonPressed(i);
        (void)isButtonLongPressed(i);
    }
}

void fsm_manual_run(void) {
    switch (status) {
        case MAN_RED1_GREEN2:
            traffic_set(1, LIGHT_RED);
            traffic_set(2, LIGHT_GREEN);

            // Nút 2 (index 1): Đảo pha qua bước đệm Vàng 2 tuyến trong 2s
            if (isButtonPressed(1)) {
                status = MAN_WAIT_TO_GREEN1_RED2;
                setTimer(0, 2000);
            }
            // Nút 3 (index 2): Quay về chế độ Tự động (giữ đúng pha Đỏ 1 - Xanh 2)
            if (isButtonPressed(2)) {
                status = AUTO_RED1_GREEN2;
                setTimer(0, time_green);
            }
            // Nút 1 (index 0): Chuyển sang Tuning (khởi đầu đèn nền theo pha Đỏ 1 - Xanh 2)
            if (isButtonPressed(0)) {
                fsm_tuning_enter(AUTO_RED1_GREEN2);
            }
            break;

        case MAN_GREEN1_RED2:
            traffic_set(1, LIGHT_GREEN);
            traffic_set(2, LIGHT_RED);

            // Nút 2 (index 1): Đảo pha qua bước đệm Vàng 2 tuyến trong 2s
            if (isButtonPressed(1)) {
                status = MAN_WAIT_TO_RED1_GREEN2;
                setTimer(0, 2000);
            }
            // Nút 3 (index 2): Quay về chế độ Tự động (giữ đúng pha Xanh 1 - Đỏ 2)
            if (isButtonPressed(2)) {
                status = AUTO_GREEN1_RED2;
                setTimer(0, time_green);
            }
            // Nút 1 (index 0): Chuyển sang Tuning (khởi đầu đèn nền theo pha Xanh 1 - Đỏ 2)
            if (isButtonPressed(0)) {
                fsm_tuning_enter(AUTO_GREEN1_RED2);
            }
            break;

        case MAN_WAIT_TO_GREEN1_RED2:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_AMBER);
            discard_all_buttons(); // Vô hiệu hóa toàn bộ nút nhấn trong 2s chờ

            if (timer_flag[0] == 1) {
                status = MAN_GREEN1_RED2;
            }
            break;

        case MAN_WAIT_TO_RED1_GREEN2:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_AMBER);
            discard_all_buttons(); // Vô hiệu hóa toàn bộ nút nhấn trong 2s chờ

            if (timer_flag[0] == 1) {
                status = MAN_RED1_GREEN2;
            }
            break;

        default:
            break;
    }
}
