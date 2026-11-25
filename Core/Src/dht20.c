#include "dht20.h"

extern I2C_HandleTypeDef hi2c1;

void dht20_init(void) {
    uint8_t status_byte = 0;
    HAL_I2C_Master_Receive(&hi2c1, DHT20_ADDR, &status_byte, 1, 100);
    if ((status_byte & 0x18) != 0x18) {
        // Khoi tao thanh ghi neu cam bien chua san sang
        uint8_t init_cmd[3] = {0xBE, 0x08, 0x00};
        HAL_I2C_Master_Transmit(&hi2c1, DHT20_ADDR, init_cmd, 3, 100);
    }
}

void dht20_trigger_measurement(void) {
    uint8_t cmd[3] = {0xAC, 0x33, 0x00};
    HAL_I2C_Master_Transmit(&hi2c1, DHT20_ADDR, cmd, 3, 100);
}

uint8_t dht20_read_data(float *temp, float *humi) {
    uint8_t rx_buf[7] = {0};
    if (HAL_I2C_Master_Receive(&hi2c1, DHT20_ADDR, rx_buf, 7, 100) != HAL_OK) {
        return 0;
    }
    // Bit 7 cua byte trang thai phai bang 0 (do xong)
    if ((rx_buf[0] & 0x80) != 0) {
        return 0;
    }

    uint32_t raw_humi = ((uint32_t)rx_buf[1] << 12) | ((uint32_t)rx_buf[2] << 4) | (rx_buf[3] >> 4);
    uint32_t raw_temp = (((uint32_t)rx_buf[3] & 0x0F) << 16) | ((uint32_t)rx_buf[4] << 8) | rx_buf[5];

    *humi = ((float)raw_humi / 1048576.0f) * 100.0f;
    *temp = ((float)raw_temp / 1048576.0f) * 200.0f - 50.0f;
    return 1;
}
