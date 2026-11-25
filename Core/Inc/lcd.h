#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "main.h"
#include <stdint.h>

#define LCD_ADDR (0x21 << 1) // Doi thanh 0x3F << 1 neu I2C scan ra 0x3F

void lcd_init(void);
void lcd_clear(void);
void lcd_goto_XY(uint8_t row, uint8_t col);
void lcd_send_string(char *str);

#endif /* INC_LCD_H_ */
