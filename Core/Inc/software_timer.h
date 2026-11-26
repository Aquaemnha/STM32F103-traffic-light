#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#include <stdint.h>

#define MAX_TIMER 5
#define TICK_MS   10  // Chu kỳ scheduler là 10ms

extern uint8_t timer_flag[MAX_TIMER];

void setTimer(uint8_t index, uint32_t duration_ms);
uint32_t getTimerRemainingMs(uint8_t index);
void timer_run(void);

#endif /* INC_SOFTWARE_TIMER_H_ */
