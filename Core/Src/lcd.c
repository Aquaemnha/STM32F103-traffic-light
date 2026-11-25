#include "lcd.h"

extern I2C_HandleTypeDef hi2c1;

static void lcd_send_cmd(char cmd) {
    char data_u = (cmd & 0xf0);
    char data_l = ((cmd << 4) & 0xf0);
    uint8_t data_t[4];
    data_t[0] = data_u | 0x0C; // en=1, rs=0
    data_t[1] = data_u | 0x08; // en=0, rs=0
    data_t[2] = data_l | 0x0C; // en=1, rs=0
    data_t[3] = data_l | 0x08; // en=0, rs=0
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_t, 4, 100);
}

static void lcd_send_data(char data) {
    char data_u = (data & 0xf0);
    char data_l = ((data << 4) & 0xf0);
    uint8_t data_t[4];
    data_t[0] = data_u | 0x0D; // en=1, rs=1
    data_t[1] = data_u | 0x09; // en=0, rs=1
    data_t[2] = data_l | 0x0D; // en=1, rs=1
    data_t[3] = data_l | 0x09; // en=0, rs=1
    HAL_I2C_Master_Transmit(&hi2c1, LCD_ADDR, data_t, 4, 100);
}

void lcd_init(void) {
    HAL_Delay(50); // Chi goi 1 lan duy nhat luc khoi tao truoc while(1)
    lcd_send_cmd(0x30); HAL_Delay(5);
    lcd_send_cmd(0x30); HAL_Delay(1);
    lcd_send_cmd(0x30); HAL_Delay(10);
    lcd_send_cmd(0x20); HAL_Delay(10);

    lcd_send_cmd(0x28); // 4-bit, 2 dong, font 5x8
    lcd_send_cmd(0x08); // Tat hien thi
    lcd_send_cmd(0x01); // Xoa man hinh
    HAL_Delay(2);
    lcd_send_cmd(0x06); // Tu dong tang con tro
    lcd_send_cmd(0x0C); // Bat hien thi, tat con tro nhap nhay
}

void lcd_clear(void) {
    lcd_send_cmd(0x01);
    HAL_Delay(2);
}

void lcd_goto_XY(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_send_cmd(address);
}

void lcd_send_string(char *str) {
    while (*str) lcd_send_data(*str++);
}
