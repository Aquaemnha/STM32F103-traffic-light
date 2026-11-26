#include "software_timer.h"

uint8_t timer_flag[MAX_TIMER] = {0};
static uint32_t timer_counter[MAX_TIMER] = {0};

void setTimer(uint8_t index, uint32_t duration_ms) {
    if (index >= MAX_TIMER) return;
    timer_counter[index] = duration_ms / TICK_MS;
    timer_flag[index] = (timer_counter[index] == 0) ? 1 : 0;
}

uint32_t getTimerRemainingMs(uint8_t index) {
    if (index >= MAX_TIMER) return 0;
    return timer_counter[index] * TICK_MS;
}

void timer_run(void) {
    for (uint8_t i = 0; i < MAX_TIMER; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] == 0) {
                timer_flag[i] = 1;
            }
        }
    }
}
