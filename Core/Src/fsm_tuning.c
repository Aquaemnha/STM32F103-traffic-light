#include "fsm_tuning.h"
#include "global.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "button.h"

static int saved_green = 3;
static int saved_amber = 2;
static int draft_green = 3;
static int draft_amber = 2;

static uint8_t tun_saved = 0;     // Cờ hiển thị chữ "SAVED" trên LCD
static uint8_t tun_modified = 0;  // Cờ đánh dấu đã xác nhận lưu trong phiên Tuning

static void discard_all_buttons(void) {
    for (uint8_t i = 0; i < 4; i++) {
        (void)isButtonPressed(i);
        (void)isButtonLongPressed(i);
    }
}

void fsm_tuning_enter(int initial_auto_phase) {
    saved_green = time_green / 1000;
    saved_amber = time_amber / 1000;
    draft_green = saved_green;
    draft_amber = saved_amber;

    tun_saved = 0;
    tun_modified = 0;

    // Nhận pha đầu tiên theo pha cuối cùng của Manual và chạy nền Auto
    auto_bg_state = initial_auto_phase;
    setTimer(0, time_green);

    status = TUN_GREEN;
}

int fsm_tuning_get_red(void) {
    return draft_green + draft_amber;
}

int fsm_tuning_get_green(void) {
    return draft_green;
}

int fsm_tuning_get_amber(void) {
    return draft_amber;
}

uint8_t fsm_tuning_is_saved(void) {
    return tun_saved;
}

void fsm_tuning_run(void) {
    int max_val = 99;

    switch (status) {
        case TUN_GREEN:
            max_val = 99 - draft_amber;
            if (max_val < 1) max_val = 1;

            // Nút 2 (index 1): Tăng thời gian đèn Xanh (khóa tại ngưỡng trần max_val)
            if (isButtonPressed(1) || isButtonLongPressed(1)) {
                tun_saved = 0;
                if (draft_green < max_val) {
                    draft_green++;
                }
            }
            // Nút 3 (index 2): Giảm thời gian đèn Xanh (từ 1 quay vòng về max_val)
            if (isButtonPressed(2) || isButtonLongPressed(2)) {
                tun_saved = 0;
                draft_green--;
                if (draft_green < 1) {
                    draft_green = max_val;
                }
            }
            // Nút 4 (index 3): Lưu nháp đèn Xanh (nếu người dùng muốn lưu ngay ở bước này)
            if (isButtonPressed(3)) {
                saved_green = draft_green;
                tun_saved = 1;
                tun_modified = 1;
            }
            // Nút 1 (index 0): Sang chỉnh đèn Vàng và GIỮ NGUYÊN giá trị draft_green vừa chỉnh
            if (isButtonPressed(0)) {
                tun_saved = 0;
                status = TUN_AMBER;
            }
            break;

        case TUN_AMBER:
            max_val = 99 - draft_green;
            if (max_val < 1) max_val = 1;

            // Nút 2 (index 1): Tăng thời gian đèn Vàng (khóa tại ngưỡng trần max_val)
            if (isButtonPressed(1) || isButtonLongPressed(1)) {
                tun_saved = 0;
                if (draft_amber < max_val) {
                    draft_amber++;
                }
            }
            // Nút 3 (index 2): Giảm thời gian đèn Vàng (từ 1 quay vòng về max_val)
            if (isButtonPressed(2) || isButtonLongPressed(2)) {
                tun_saved = 0;
                draft_amber--;
                if (draft_amber < 1) {
                    draft_amber = max_val;
                }
            }
            // Nút 4 (index 3): Lưu đồng thời cả giá trị mới của đèn Xanh và đèn Vàng
            if (isButtonPressed(3)) {
                saved_green = draft_green;
                saved_amber = draft_amber;
                tun_saved = 1;
                tun_modified = 1;
            }
            // Nút 1 (index 0): Thoát Tuning về chế độ Tự động
            if (isButtonPressed(0)) {
                tun_saved = 0;
                if (tun_modified == 1) {
                    time_green = saved_green * 1000;
                    time_amber = saved_amber * 1000;
                    time_red   = time_green + time_amber;
                    status = TUN_WAIT_AUTO;
                    setTimer(0, 2000);
                } else {
                    // Chưa bấm Save lần nào -> giữ nguyên nhịp Auto đang chạy nền
                    status = auto_bg_state;
                }
            }
            break;

        case TUN_WAIT_AUTO:
            traffic_set(1, LIGHT_AMBER);
            traffic_set(2, LIGHT_AMBER);
            discard_all_buttons(); // Vô hiệu hóa nút nhấn trong 2s chờ

            if (timer_flag[0] == 1) {
                status = AUTO_RED1_GREEN2;
                setTimer(0, time_green);
            }
            break;

        default:
            break;
    }
}
