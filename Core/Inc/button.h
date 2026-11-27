#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"
#include <stdint.h>

#define NUM_BUTTONS     4
#define PRESSED_STATE   GPIO_PIN_RESET
#define NORMAL_STATE    GPIO_PIN_SET

void button_reading(void);
uint8_t isButtonPressed(uint8_t index);
uint8_t isButtonLongPressed(uint8_t index);

#endif /* INC_BUTTON_H_ */
