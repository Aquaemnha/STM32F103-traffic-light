#ifndef INC_FSM_TUNING_H_
#define INC_FSM_TUNING_H_

#include <stdint.h>

void fsm_tuning_enter(int initial_auto_phase);
void fsm_tuning_run(void);

int fsm_tuning_get_red(void);
int fsm_tuning_get_green(void);
int fsm_tuning_get_amber(void);
uint8_t fsm_tuning_is_saved(void);

#endif /* INC_FSM_TUNING_H_ */
