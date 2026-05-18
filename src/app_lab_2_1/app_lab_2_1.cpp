#include "app_lab_2_1.h"
#include "serial_stdio/serial_stdio.h"
#include "srv_os_seq/srv_os_seq.h"
#include "MyTasks.h"
#include <stdio.h>
#include <Arduino.h>


void app_lab_2_1_setup() 
{ 
    serial_stdio_setup();
    srv_sys_os_timer_setup();
    srv_os_seq_setup();
    printf("System initialized. Starting scheduler...\r\n");
}

void app_lab_2_1_run()
{
    srv_os_seq_run();
    run_idle_task();
}                  