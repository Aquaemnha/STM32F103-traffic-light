#include "scheduler.h"
#include "stm32f1xx.h"

static volatile sTask SCH_tasks_G[SCH_MAX_TASKS];
static volatile uint8_t SCH_count = 0;
static uint32_t next_task_id = 1;

/* TIM2 sees either queue layout, never an array partway through a shift. */
static uint32_t lock_queue(void) {
    uint32_t mask = __get_PRIMASK();
    __disable_irq();
    return mask;
}

void SCH_Init(void) {
    SCH_count = 0;
    for (uint8_t i = 0; i < SCH_MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = NO_TASK_ID;
    }
}

// Ham cap nhat O(1): chi giam Delay cua phan tu dau hang doi
void SCH_Update(void) {
    if (SCH_count == 0) return;
    if (SCH_tasks_G[0].Delay > 0) {
        SCH_tasks_G[0].Delay--;
    }
    if (SCH_tasks_G[0].Delay == 0) {
        SCH_tasks_G[0].RunMe = 1;
    }
}

uint32_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
    if (pFunction == 0) {
        return NO_TASK_ID;
    }
    uint32_t mask = lock_queue();
    if (SCH_count >= SCH_MAX_TASKS) {
        __set_PRIMASK(mask);
        return NO_TASK_ID;
    }

    uint8_t pos = 0;
    uint32_t rel_delay = DELAY;

    // Tim vi tri chen phu hop va tinh Delay tuong doi
    while (pos < SCH_count && rel_delay >= SCH_tasks_G[pos].Delay) {
        rel_delay -= SCH_tasks_G[pos].Delay;
        pos++;
    }

    // Dich phai cac task phia sau vi tri chen
    for (uint8_t i = SCH_count; i > pos; i--) {
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];
    }

    // Cap nhat lai Delay tuong doi cua task dung ngay sau task moi
    if (pos < SCH_count) {
        SCH_tasks_G[pos + 1].Delay -= rel_delay;
    }

    uint32_t assigned_id = next_task_id++;
    if (next_task_id == NO_TASK_ID) next_task_id = 1;

    SCH_tasks_G[pos].pTask  = pFunction;
    SCH_tasks_G[pos].Delay  = rel_delay;
    SCH_tasks_G[pos].Period = PERIOD;
    SCH_tasks_G[pos].RunMe  = (rel_delay == 0) ? 1 : 0;
    SCH_tasks_G[pos].TaskID = assigned_id;

    SCH_count++;
    __set_PRIMASK(mask);
    return assigned_id;
}

uint8_t SCH_Delete_Task(uint32_t taskID) {
    if (taskID == NO_TASK_ID) return 0;
    uint32_t mask = lock_queue();

    for (uint8_t i = 0; i < SCH_count; i++) {
        if (SCH_tasks_G[i].TaskID == taskID) {
            // Cong don Delay cua task bi xoa cho task ke tiep de giu dung moc thoi gian
            if (i < SCH_count - 1) {
                SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;
                if (i == 0) {
                    /* A tied task inherits the deleted head's time to deadline. */
                    SCH_tasks_G[1].RunMe = (SCH_tasks_G[1].Delay == 0);
                }
            }
            // Dich trai mang
            for (uint8_t j = i; j < SCH_count - 1; j++) {
                SCH_tasks_G[j] = SCH_tasks_G[j + 1];
            }
            SCH_count--;
            SCH_tasks_G[SCH_count].pTask = 0;
            SCH_tasks_G[SCH_count].TaskID = NO_TASK_ID;
            __set_PRIMASK(mask);
            return 1;
        }
    }
    __set_PRIMASK(mask);
    return 0;
}

void SCH_Dispatch_Tasks(void) {
    while (SCH_count > 0 && SCH_tasks_G[0].RunMe > 0) {
        void (*task_to_run)(void) = SCH_tasks_G[0].pTask;
        uint32_t period = SCH_tasks_G[0].Period;
        uint32_t id = SCH_tasks_G[0].TaskID;

        // Xoa task khoi dau danh sach truoc khi chay
        SCH_Delete_Task(id);

        // Thuc thi ham task
        if (task_to_run != 0) {
            (*task_to_run)();
        }

        // Neu Period > 0 thi lap lich lai; Period == 0 (one-shot) thi tu huy
        if (period > 0) {
            SCH_Add_Task(task_to_run, period, period);
        }
    }
}
