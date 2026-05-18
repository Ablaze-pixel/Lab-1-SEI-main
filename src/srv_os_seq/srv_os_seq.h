#ifndef SRV_OS_SEQ_H
#define SRV_OS_SEQ_H

#include <Arduino.h>

void srv_sys_os_timer_setup();
void srv_sys_os_timer_scheduler();
void srv_os_seq_setup();
void srv_os_seq_run();

#endif