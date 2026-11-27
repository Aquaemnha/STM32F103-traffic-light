#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include <stdint.h>

// Định nghĩa các trạng thái của toàn hệ thống
enum SystemState {
    INIT,
    // Chế độ tự động (Automatic)
    AUTO_RED1_GREEN2,
    AUTO_RED1_AMBER2,
    AUTO_GREEN1_RED2,
    AUTO_AMBER1_RED2,
    // Chế độ bằng tay (Manual)
    MAN_RED1_GREEN2,
    MAN_GREEN1_RED2,
    MAN_WAIT_TO_RED1_GREEN2,
    MAN_WAIT_TO_GREEN1_RED2,
    // Chế độ chỉnh thời gian (Tuning)
    TUN_RED,
    TUN_AMBER,
    TUN_GREEN,
    TUN_WAIT_AUTO
};

extern int status;
extern int auto_bg_state;

// Thời gian đèn chính thức (đơn vị: ms)
extern int time_red;
extern int time_amber;
extern int time_green;

// Biến tạm dùng khi chỉnh thời gian (đơn vị: giây, từ 1 -> 99)
extern int temp_time;

// Dữ liệu cảm biến môi trường
extern float temperature;
extern float humidity;

#endif /* INC_GLOBAL_H_ */
