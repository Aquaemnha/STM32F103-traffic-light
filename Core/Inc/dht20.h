#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include "main.h"
#include <stdint.h>

#define DHT20_ADDR (0x38 << 1)

void dht20_init(void);
void dht20_trigger_measurement(void);
uint8_t dht20_read_data(float *temp, float *humi);

#endif /* INC_DHT20_H_ */
