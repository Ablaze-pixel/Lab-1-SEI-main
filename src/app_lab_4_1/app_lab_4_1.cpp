#include "app_lab_4_1.h"
#include "lcd_stdio/lcd_stdio.h"
#include "keypad_stdio/keypad_stdio.h"
#include "dd_relay/dd_relay.h"
#include <stdio.h>
#include <string.h>

// Constante pentru configurare (evitarea magic numbers)
#define BUF_SIZE      16
#define CMD_CLEAR_LCD "#"
#define KEY_ENTER     '#'
#define KEY_RESET     '*'

static char cmd_buffer[BUF_SIZE];
static int cmd_idx = 0;

/**
 * Șterge buffer-ul și resetează indexul.
 */
static void reset_input_state(void) {
    cmd_idx = 0;
    memset(cmd_buffer, 0, BUF_SIZE);
}

/**
 * Procesează comanda acumulată în buffer.
 */
static void process_relay_command(void) {
    printf(CMD_CLEAR_LCD); // Clear LCD înainte de afișarea rezultatului

    if (strcasecmp(cmd_buffer, "A") == 0 || strcasecmp(cmd_buffer, "ON") == 0) {
        dd_relay_on();
        printf("Relay -> ON\r\n");
    } 
    else if (strcasecmp(cmd_buffer, "B") == 0 || strcasecmp(cmd_buffer, "OFF") == 0) {
        dd_relay_off();
        printf("Relay -> OFF\r\n");
    } 
    else {
        printf("Unknown: %s\r\n", cmd_buffer);
    }
}

void app_lab_4_1_setup(void) {
    dd_relay_setup();
    lcd_stdio_setup();
    keypad_stdio_setup();
    
    printf(CMD_CLEAR_LCD);
    printf("Relay System\r\n");
    reset_input_state();
}

void app_lab_4_1_run(void) {
    char key;

    // Citire non-blocking (presupunând că scanf este configurat astfel în stdio)
    if (scanf("%c", &key) != 1) {
        return;
    }

    if (key == KEY_ENTER) {
        cmd_buffer[cmd_idx] = '\0';
        process_relay_command();
        reset_input_state();
    } 
    else if (key == KEY_RESET) {
        printf(CMD_CLEAR_LCD);
        printf("Cleared\r\n");
        reset_input_state();
    } 
    else {
        // Adăugare în buffer dacă mai este spațiu (rezervăm 1 byte pentru null-terminator)
        if (cmd_idx < (BUF_SIZE - 1)) {
            cmd_buffer[cmd_idx++] = key;
            printf("%c", key); // Echo la caracterul tastat
        }
    }
}