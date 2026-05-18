#include "app_lab_1_2.h"
#include "lcd_stdio/lcd_stdio.h"
#include "keypad_stdio/keypad_stdio.h"
#include "dd_led/dd_led.h"
#include <stdio.h>

#define LED_GREEN 0
#define LED_RED 1
#define PASSWORD_LENGHT 4

char code[PASSWORD_LENGHT] = {'1','2','3','4'};
char input[PASSWORD_LENGHT];
int nr_input = 0;
char key;

void app_lab_1_2_setup() 
{ 
    dd_led_setup();
    lcd_stdio_setup();
    keypad_stdio_setup();
}

void app_lab_1_2_run()
{
    
    scanf("%c", &key);
    
    if(key == '#')
    {
        printf("#");
        nr_input = 0;
        dd_led_off(LED_RED);
        dd_led_off(LED_GREEN);
        return;
    }

    printf("%c", key);
    input[nr_input] = key;
    nr_input++;

    if(nr_input == PASSWORD_LENGHT)
    {
        if(strncmp(code, input, PASSWORD_LENGHT) == 0)
        {
            dd_led_on(LED_GREEN);
            dd_led_off(LED_RED);
        }
        else
        {
            dd_led_on(LED_RED);
            dd_led_off(LED_GREEN);
        }
        nr_input = 0;
        printf("#");
    }


    
}