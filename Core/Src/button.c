#include "button.h"

static GPIO_TypeDef* btn_ports[NUM_BUTTONS] = {
    BTN_0_GPIO_Port, BTN_1_GPIO_Port, BTN_2_GPIO_Port, BTN_3_GPIO_Port
};
static uint16_t btn_pins[NUM_BUTTONS] = {
    BTN_0_Pin, BTN_1_Pin, BTN_2_Pin, BTN_3_Pin
};

static GPIO_PinState KeyReg0[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
static GPIO_PinState KeyReg1[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
static GPIO_PinState KeyReg2[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};
static GPIO_PinState KeyReg3[NUM_BUTTONS] = {NORMAL_STATE, NORMAL_STATE, NORMAL_STATE, NORMAL_STATE};

static uint16_t TimeOutForKeyPress[NUM_BUTTONS] = {50, 50, 50, 50};
static uint8_t button_flag[NUM_BUTTONS] = {0};
static uint8_t button_long_flag[NUM_BUTTONS] = {0};

void button_reading(void) {
    for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
        KeyReg2[i] = KeyReg1[i];
        KeyReg1[i] = KeyReg0[i];
        KeyReg0[i] = HAL_GPIO_ReadPin(btn_ports[i], btn_pins[i]);

        if ((KeyReg1[i] == KeyReg0[i]) && (KeyReg1[i] == KeyReg2[i])) {
            if (KeyReg2[i] != KeyReg3[i]) {
                KeyReg3[i] = KeyReg2[i];
                if (KeyReg3[i] == PRESSED_STATE) {
                    TimeOutForKeyPress[i] = 50; // Chờ 500ms để bắt đầu nhấn giữ
                    button_flag[i] = 1;
                }
            } else {
                // Nhấn giữ: tự động kích mỗi 50ms (20 lần/giây)
                if (KeyReg3[i] == PRESSED_STATE) {
                    TimeOutForKeyPress[i]--;
                    if (TimeOutForKeyPress[i] == 0) {
                        TimeOutForKeyPress[i] = 5;
                        button_long_flag[i] = 1;
                    }
                }
            }
        }
    }
}

uint8_t isButtonPressed(uint8_t index) {
    if (index >= NUM_BUTTONS) return 0;
    if (button_flag[index] == 1) {
        button_flag[index] = 0;
        return 1;
    }
    return 0;
}

uint8_t isButtonLongPressed(uint8_t index) {
    if (index >= NUM_BUTTONS) return 0;
    if (button_long_flag[index] == 1) {
        button_long_flag[index] = 0;
        return 1;
    }
    return 0;
}
