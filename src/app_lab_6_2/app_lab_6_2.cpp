#include "app_lab_6_2.h"
#include "fsm/fsm.h"
#include "dd_button/dd_button.h"
#include "dd_led/dd_led.h"
#include "serial_stdio/serial_stdio.h"
#include <stdio.h>
#include <Arduino.h>
#include <Arduino_FreeRTOS.h> 

#define BUTTON_NORTH_SENSOR 0 
#define LED_EAST_GREEN 2
#define LED_EAST_YELLOW 3
#define LED_EAST_RED 4
#define LED_NORTH_GREEN 5
#define LED_NORTH_YELLOW 6
#define LED_NORTH_RED 7

static fsm_t traffic_fsm;

// ============================================================================
// FUNCȚIILE DE STARE (User Actions - Schimbarea becurilor fizice)
// ============================================================================
static void action_east_green_north_red(void) {
    printf("\r\n[STDIO] STATE: EAST=GREEN, NORTH=RED\r\n");
    dd_led_on(LED_EAST_GREEN); dd_led_on(LED_NORTH_RED);
    dd_led_off(LED_EAST_YELLOW); dd_led_off(LED_EAST_RED);
    dd_led_off(LED_NORTH_GREEN); dd_led_off(LED_NORTH_YELLOW);
}

static void action_east_yellow_north_red(void) {
    printf("\r\n[STDIO] STATE: EAST=YELLOW, NORTH=RED\r\n");
    dd_led_on(LED_EAST_YELLOW); dd_led_on(LED_NORTH_RED);
    dd_led_off(LED_EAST_GREEN); dd_led_off(LED_EAST_RED);
    dd_led_off(LED_NORTH_GREEN); dd_led_off(LED_NORTH_YELLOW);
}

static void action_east_red_north_green(void) {
    printf("\r\n[STDIO] STATE: EAST=RED, NORTH=GREEN\r\n");
    dd_led_on(LED_EAST_RED); dd_led_on(LED_NORTH_GREEN);
    dd_led_off(LED_EAST_GREEN); dd_led_off(LED_EAST_YELLOW);
    dd_led_off(LED_NORTH_RED); dd_led_off(LED_NORTH_YELLOW);
}

static void action_east_red_north_yellow(void) {
    printf("\r\n[STDIO] STATE: EAST=RED, NORTH=YELLOW\r\n");
    dd_led_on(LED_EAST_RED); dd_led_on(LED_NORTH_YELLOW);
    dd_led_off(LED_EAST_GREEN); dd_led_off(LED_EAST_YELLOW);
    dd_led_off(LED_NORTH_RED); dd_led_off(LED_NORTH_GREEN);
}

// ============================================================================
// TABELUL DE TRANZIȚIE PUR (FSM Table Map)
// ============================================================================
static const fsm_transition_t traffic_transition_table[] = {
    // Stare Curentă             | Eveniment primit       | Stare Următoare             | Funcție executată
    { STATE_EAST_GREEN_NORTH_RED,  EVENT_TRAFFIC_REQUEST,   STATE_EAST_YELLOW_NORTH_RED,  action_east_yellow_north_red  },
    { STATE_EAST_YELLOW_NORTH_RED, EVENT_STEP_SEQUENCE,     STATE_EAST_RED_NORTH_GREEN,   action_east_red_north_green   },
    { STATE_EAST_RED_NORTH_GREEN,  EVENT_STEP_SEQUENCE,     STATE_EAST_RED_NORTH_YELLOW,  action_east_red_north_yellow  },
    { STATE_EAST_RED_NORTH_YELLOW, EVENT_STEP_SEQUENCE,     STATE_EAST_GREEN_NORTH_RED,   action_east_green_north_red   }
};

#define TRANSITIONS_COUNT (sizeof(traffic_transition_table) / sizeof(traffic_transition_table[0]))

// ============================================================================
// INTEGRAREA FREERTOS (Controlul timpului și al senzorilor)
// ============================================================================
static void vTrafficLightTask(void *pvParameters) {
    (void) pvParameters;

    // Setăm starea inițială vizuală la pornire
    action_east_green_north_red();

    for (;;) {
        // REGULA 1: Dacă Estul e Verde, stăm și așteptăm cererea de la Nord (Prioritate Est)
        if (traffic_fsm.current_state == STATE_EAST_GREEN_NORTH_RED) {
            
            // Folosim funcția stabilă de la Lab 6.1 care așteaptă eliberarea
            if (dd_button_read_and_wait(BUTTON_NORTH_SENSOR) == 1) {
                printf("[Sensor] Active request on NORTH direction detected!\r\n");
                
                // Trimitem evenimentul de cerere -> FSM trece în Yellow
                fsm_process_event(&traffic_fsm, EVENT_TRAFFIC_REQUEST);
                
                // Controlăm timpul de afișare pentru Galben prin FreeRTOS
                vTaskDelay(2000 / portTICK_PERIOD_MS); 
            }
        }
        // REGULA 2: Dacă am ieșit din starea prioritară, executăm restul tranzițiilor pe bază de timp
        else {
            // Trimitem semnalul de pas următor
            fsm_process_event(&traffic_fsm, EVENT_STEP_SEQUENCE);

            // Verificăm în ce stare a ajuns FSM-ul ca să știm cât timp blocăm task-ul
            if (traffic_fsm.current_state == STATE_EAST_RED_NORTH_GREEN) {
                vTaskDelay(5000 / portTICK_PERIOD_MS); // Nord stă Verde 5 secunde
            } 
            else if (traffic_fsm.current_state == STATE_EAST_RED_NORTH_YELLOW) {
                vTaskDelay(2000 / portTICK_PERIOD_MS); // Nord stă Galben 2 secunde
            }
            else if (traffic_fsm.current_state == STATE_EAST_GREEN_NORTH_RED) {
                // S-a întors la Verde pe Est. Oferim un timp minim de siguranță (ex: 3 secunde) 
                // în care Estul rămâne verde obligatoriu înainte de a permite o nouă scanare de buton
                vTaskDelay(3000 / portTICK_PERIOD_MS);
            }
        }

        // Un delay mic de veghe pentru a nu suprasolicita nucleul FreeRTOS
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

// ============================================================================
// INIȚIALIZARE
// ============================================================================
void app_lab_6_2_setup(void) {
    serial_stdio_setup();
    dd_button_setup();
    dd_led_setup();

    // Inițializăm FSM-ul pur cu tabelul de tranziții
    fsm_init(&traffic_fsm, STATE_EAST_GREEN_NORTH_RED, traffic_transition_table, TRANSITIONS_COUNT);

    printf("\r\n=== LAB 6.2: PURE FSM + FREERTOS TRAFFIC LIGHT INITIALIZED ===\r\n");

    // Creăm task-ul FreeRTOS care va gestiona timpul dintre tranziții
    xTaskCreate(vTrafficLightTask, "TrafficTask", 128, NULL, 1, NULL);
}

void app_lab_6_2_run(void) {
    // FreeRTOS Scheduler preia controlul executiei
}