#include "dd_l298/dd_l298.h"
#include "app_lab_4_2.h"
#include "serial_stdio/serial_stdio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Arduino.h>

#define BUF_SIZE      32

static char cmd_buffer[BUF_SIZE];
static int cmd_idx = 0;

static void reset_input_state(void) {
    cmd_idx = 0;
    memset(cmd_buffer, 0, BUF_SIZE);
}

static void report_system_status(void) {
    int16_t pwr = dd_l298_get_power();

    printf("=== CONTROL MOTOR VIA SERIAL ===\r\n");
    if (pwr == 0) {
        printf("Status: STOP,  Putere: 0%%\r\n");
    } else {
        printf("Status: %s,    Putere: %d%%\r\n", (pwr > 0) ? "FORWARD" : "REVERSE", pwr);
    }
    printf("Introdu comanda: ");
}


static void process_motor_command(void) {
    // 1. motor set [-100 .. 100]
    if (strncasecmp(cmd_buffer, "motor set ", 10) == 0) {
        int val = atoi(cmd_buffer + 10);
        dd_l298_set_power(val);
    }
    else if (strcasecmp(cmd_buffer, "motor stop") == 0) {
        dd_l298_stop();
    }
    else if (strcasecmp(cmd_buffer, "motor max") == 0) {
        dd_l298_max();
    }
    else if (strcasecmp(cmd_buffer, "motor inc") == 0) {
        dd_l298_inc();
    }
    else if (strcasecmp(cmd_buffer, "motor dec") == 0) {
        dd_l298_dec();
    }
    // Comandă eronată
    else {
        printf("\r\nEroare: Comanda [%s]\r\n", cmd_buffer);
        delay(500);
    }

    report_system_status();
}


void app_lab_4_2_setup(void) {
    serial_stdio_setup(); 
    dd_l298_setup();      
    
    reset_input_state();
    report_system_status();
}


void app_lab_4_2_run(void) 
{
    char key;

    if (scanf("%c", &key) != 1) 
    {
        return; 
    }

    if (key == '\r' || key == '\n') {
        if (cmd_idx > 0) 
        {
            cmd_buffer[cmd_idx] = '\0'; 
            process_motor_command();
            reset_input_state();
        }
    } 

    else if (key >= 32 && key <= 126) {
        if (cmd_idx < (BUF_SIZE - 1)) 
        {
            cmd_buffer[cmd_idx++] = key;
            printf("%c", key); // Echo
        }
    }
}