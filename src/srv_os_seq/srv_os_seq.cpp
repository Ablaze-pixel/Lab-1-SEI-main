#include "app_lab_2_1/MyTasks.h"
#include "serial_stdio/serial_stdio.h"
#include "srv_os_seq.h"
#include "Arduino.h"

#include "timer-api.h"

#define SYSTEM_TICK 1 
#define TASK_1_RECURENCY 50
#define TASK_1_OFFSET 100
int task_1_counter = TASK_1_OFFSET;

#define TASK_2_RECURENCY 100
#define TASK_2_OFFSET 200
int task_2_counter = TASK_2_OFFSET;

#define TASK_3_RECURENCY 200
#define TASK_3_OFFSET 300
int task_3_counter = TASK_3_OFFSET;


// Timer de 1 ms
void srv_sys_os_timer_setup(){
    TCCR1B |= (1 << WGM12); // CTC timer mode
    OCR1A = 16000; // Compare value for 1ms at 16MHz with prescaler 64
    TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
    TCCR1B |= (1 << CS11) | (1 << CS10);
}
void srv_sys_os_timer_scheduler(){
    if(--task_1_counter <= 0){
        task_1_counter = TASK_1_RECURENCY;
        task_1_run();
    }
    if(--task_2_counter <= 0){
        task_2_counter = TASK_2_RECURENCY;
        task_2_run();
    }
    if(--task_3_counter <= 0){
        task_3_counter = TASK_3_RECURENCY;
        task_3_run();
    }
}

ISR(TIMER1_COMPA_vect){
    srv_sys_os_timer_scheduler();
}

void srv_os_seq_setup() {
    task_1_setup();
    task_1_counter = TASK_1_OFFSET;

    task_2_setup();
    task_2_counter = TASK_2_OFFSET;

    task_3_setup();
    task_3_counter = TASK_3_OFFSET;
}

void srv_os_seq_run() {

    delay(SYSTEM_TICK); //do nothing, just wait for the timer interrupt to call the scheduler
}