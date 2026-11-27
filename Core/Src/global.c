#include "global.h"

int status = INIT;
int auto_bg_state = AUTO_RED1_GREEN2;

// Ràng buộc mặc định: time_red = time_green + time_amber (5s = 3s + 2s)
int time_red   = 5000;
int time_amber = 2000;
int time_green = 3000;

int temp_time  = 1;

float temperature = 0.0f;
float humidity    = 0.0f;
