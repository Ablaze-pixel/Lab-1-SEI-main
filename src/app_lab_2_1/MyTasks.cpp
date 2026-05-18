#include "MyTasks.h"
#include "dd_led/dd_led.h"
#include "dd_button/dd_button.h"

#define LED_GREEN 0
#define LED_RED   1
#define BUTTON_1  0
#define BUTTON_2  1 
#define BUTTON_3  2


 bool LED_GREEN_ACTIVE = false; 

 int Blink_Period = 5; // Valoarea default (5 tick-uri)

 bool LED_RED_ACTIVE = false;

void task_1_setup() 
{
    dd_led_setup();
    dd_button_setup();
}

void task_1_run() 
{
    if (dd_button_pressed(BUTTON_1) == true) {
        dd_led_off(LED_GREEN);
        LED_GREEN_ACTIVE = false;//GLOBAL
    } else {
        dd_led_on(LED_GREEN);
        LED_GREEN_ACTIVE = true;//GLOBAL
    }
    // printf("Task1 running\r\n");
    
}

void task_2_setup() 
{

}

void task_2_run() 
{
    static int tick_counter = 0;
    if(dd_led_read_state(LED_GREEN) == false && LED_GREEN_ACTIVE == false)
    {
       tick_counter++;
        
        // CONSUMER: Depinde de Blink_Period
        if (tick_counter >= Blink_Period) {
            LED_RED_ACTIVE = !LED_RED_ACTIVE;
            
            if (LED_RED_ACTIVE) dd_led_on(LED_RED);
            else dd_led_off(LED_RED);
            
            tick_counter = 0;
        }
    } else {

        dd_led_off(LED_RED);
        LED_RED_ACTIVE = false;
        tick_counter = 0;
    }
    // printf("Task2 running\r\n");
    
}

void task_3_setup() 
{

}

void task_3_run() 
{
   static bool last_btn_inc = false;
    static bool last_btn_dec = false;

    bool current_inc = dd_button_pressed(BUTTON_2); // Pin A9
    bool current_dec = dd_button_pressed(BUTTON_3); // Pin A10

    // Incrementare la apăsare (Edge detection)
    if (current_inc && !last_btn_inc) {
        Blink_Period++;
    }
    // Decrementare la apăsare
    if (current_dec && !last_btn_dec) {
        Blink_Period--;
    }

    // Limite de siguranță pentru variabilă
    if (Blink_Period < 1) Blink_Period = 1;
    if (Blink_Period > 50) Blink_Period = 50;

    last_btn_inc = current_inc;
    last_btn_dec = current_dec; 
}

void run_idle_task() {
   static unsigned long last_report = 0;
    
    // Raportare la fiecare 1000ms folosind millis()
    if (millis() - last_report >= 1000) {

        printf("\r\n--- SYSTEM MONITOR ---\n");
        printf("\rTask1 (LED_G): %s\n", LED_GREEN_ACTIVE ? "ON" : "OFF");
        printf("\rTask3 (Period): %d tick-uri\n", Blink_Period);
        printf("\rTask2 (LED_R): %s\n", LED_RED_ACTIVE ? "ACTIVE" : "INACTIVE");
        printf("\r----------------------\n");
        
        last_report = millis();
    }
}