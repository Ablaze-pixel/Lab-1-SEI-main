#ifndef MY_TASKS_H
#define MY_TASKS_H
#include <Arduino.h>

void task_1_setup_1();
void task_1(void *pvParameters);
void task_2_setup_1();
void task_2(void *pvParameters);
// void task_3_setup_1();
void task_3(void *pvParameters);
void run_idle_task_1();

#endif 