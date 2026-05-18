#include <Arduino.h>
#include "app_lab_1_1/app_lab_1_1.h"
#include "dd_led/dd_led.h"
#include "serial_stdio/serial_stdio.h"
#include <string.h>

#define CMD_BUFFER 32
#define LED_ID 0

static char commandBuffer[CMD_BUFFER];

void app_lab_1_1_setup(void)
{
   serial_stdio_setup();
   dd_led_setup();
   printf("App Initialized. Enter commands to control the LED (on, off, toggle).\r\n");
}




void app_lab_1_1_run(void)
{
   printf("Enter command:Led ON, OFF, TOGGLE: \r\n");
   scanf("%s", commandBuffer);
   printf("Command: %s\r\n", commandBuffer);
   
   if(strcmp(commandBuffer, "on") == 0)
   {
       dd_led_on(LED_ID);
       printf("LED turned ON\r\n");
   }
   else if(strcmp(commandBuffer, "off") == 0)
   {
       dd_led_off(LED_ID);
       printf("LED turned OFF\r\n");
   }
   else if(strcmp(commandBuffer, "toggle") == 0)
   {
       dd_led_toggle(LED_ID);
       printf("LED toggled\r\n");
   }
   else
   {
       printf("Unknown command: %s\r\n", commandBuffer);
   }
    delay(1000);
   
}