#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "main.h"
#include <stdint.h>

enum LightColor {
    LIGHT_OFF = 0,
    LIGHT_RED,
    LIGHT_AMBER,
    LIGHT_GREEN
};

void traffic_set(uint8_t road, uint8_t color);

#endif /* INC_TRAFFIC_LIGHT_H_ */
