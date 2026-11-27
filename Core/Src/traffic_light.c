#include "traffic_light.h"

// Gia su bang chan tri phan cung do duoc:
// (0,0) = OFF, (1,0) = RED, (0,1) = AMBER, (1,1) = GREEN
// (Cap nhat lai 4 case ben duoi neu module thuc te co to hop bit khac)
void traffic_set(uint8_t road, uint8_t color) {
    GPIO_PinState bitA = GPIO_PIN_RESET;
    GPIO_PinState bitB = GPIO_PIN_RESET;

    switch (color) {
        case LIGHT_OFF:   bitA = GPIO_PIN_RESET; bitB = GPIO_PIN_RESET; break;
        case LIGHT_GREEN:   bitA = GPIO_PIN_SET;   bitB = GPIO_PIN_RESET; break;
        case LIGHT_AMBER: bitA = GPIO_PIN_RESET; bitB = GPIO_PIN_SET;   break;
        case LIGHT_RED: bitA = GPIO_PIN_SET;   bitB = GPIO_PIN_SET;   break;
        default: break;
    }

    if (road == 1) {
        HAL_GPIO_WritePin(TL1_A_GPIO_Port, TL1_A_Pin, bitA);
        HAL_GPIO_WritePin(TL1_B_GPIO_Port, TL1_B_Pin, bitB);
    } else if (road == 2) {
        HAL_GPIO_WritePin(TL2_A_GPIO_Port, TL2_A_Pin, bitA);
        HAL_GPIO_WritePin(TL2_B_GPIO_Port, TL2_B_Pin, bitB);
    }
}
