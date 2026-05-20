#include "app_lab_6_1.h"
#include "fsm/fsm.h"
#include "dd_button/dd_button.h"
#include "dd_led/dd_led.h"
#include "serial_stdio/serial_stdio.h"
#include <stdio.h>
#include <Arduino.h>

#define LED_GREEN 0
#define BUTTON_1  0

static fsm_t led_fsm;

// ============================================================================
// FUNCȚIILE DE STARE (User Defined Functions)
// Utilizatorul scrie doar aceste funcții simple care controlează comportamentul
// ============================================================================
static void state_action_off(void) {
    dd_led_off(LED_GREEN);
    printf("\r\nSTATE: STATE_OFF.\r\n");
}

static void state_action_on(void) {
    dd_led_on(LED_GREEN);
    printf("\r\nSTATE: STATE_ON.\r\n");
}

// ============================================================================
// TABELUL DE TRANZIȚIE (FSM Table Map)
// Utilizatorul doar pune valorile și asociază funcțiile cu evenimentul
// ============================================================================
static const fsm_transition_t lab_transition_table[] = {
    // Stare Curentă | Eveniment Primit     | Stare Următoare | Funcție de executat
    { STATE_OFF,       EVENT_BUTTON_PRESSED,  STATE_ON,         state_action_on  },
    { STATE_ON,        EVENT_BUTTON_PRESSED,  STATE_OFF,        state_action_off }
};

#define TRANSITIONS_COUNT (sizeof(lab_transition_table) / sizeof(lab_transition_table[0]))

// ============================================================================
// LOGICA DE CONTROL
// ============================================================================

void app_lab_6_1_setup(void) {
    serial_stdio_setup();
    dd_button_setup();
    dd_led_setup();
    
    // Forțăm starea inițială hardware
    dd_led_off(LED_GREEN);
    
    // Inițializăm FSM-ul cu tabelul nostru pur de valori
    fsm_init(&led_fsm, STATE_OFF, lab_transition_table, TRANSITIONS_COUNT);
    
    printf("\r\n=== FSM ===\r\n");
    printf("Initial State: STATE_OFF\r\n");
}

void app_lab_6_1_run(void) {
    fsm_event_t system_event = FSM_EVENT_NONE;

    // Folosim funcția cea nouă de verificare și așteptare din driver
    if (dd_button_read_and_wait(BUTTON_1) == 1) {
        system_event = EVENT_BUTTON_PRESSED;
    }

    // Procesatorul generic FSM preia evenimentul și execută switch-ul corect
    fsm_process_event(&led_fsm, system_event);
}