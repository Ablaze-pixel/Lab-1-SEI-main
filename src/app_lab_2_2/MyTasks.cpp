#include "app_lab_2_2/MyTasks.h"
#include "dd_led/dd_led.h"
#include "dd_button/dd_button.h"
#include "Arduino_FreeRTOS.h"
#include "semphr.h"

#define LED_GREEN 0
#define LED_RED   1
#define BUTTON_1  0
#define BUTTON_2  1 
#define BUTTON_3  2


#define ONE_SECOND 1000
#define TASK_1_RECURRENCE 1000
#define TASK_1_OFFSET 100

static SemaphoreHandle_t xSemaphore = NULL;
static QueueHandle_t xQueue = NULL;
static int task1_led_time_counter = -1;
static int task2_N_counter = 0;

void task_1_setup_1()
{
    // definirea semaforului
    xSemaphore = xSemaphoreCreateBinary();
}

void task_1(void *pvParameters) 
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    vTaskDelay(TASK_1_OFFSET / portTICK_PERIOD_MS);

    while(1)
    {

        if(dd_button_pressed(BUTTON_1))
        {
            task1_led_time_counter = ONE_SECOND / TASK_1_RECURRENCE;
            dd_led_on(LED_GREEN);
            printf("Task 1: Button 1 pressed\r\n");
        }

        if(task1_led_time_counter > 0)
        {
            task1_led_time_counter--;
            dd_led_on(LED_GREEN);
            printf("Task 1: LED GREEN ON\r\n");
        }
        else if(task1_led_time_counter == 0)
        {
            dd_led_off(LED_GREEN);
            printf("Task 1: LED GREEN OFF\r\n");
            // ridicam semaforul
            xSemaphoreGive(xSemaphore);
            printf("Task 1: Semaphore given\r\n");
            task1_led_time_counter--;
        }

        vTaskDelayUntil(&xLastWakeTime, TASK_1_RECURRENCE / portTICK_PERIOD_MS);
    } 
}

void task_2_setup_1()
{
    // creem un buffer coada
    xQueue = xQueueCreate(100, sizeof(int));
}

void task_2(void *pvParameters) 
{

    while(1)
    {
        //asteptam semaforul
        if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE)
        {
            task2_N_counter++;
            printf("Task 2: Semaphore taken\r\n");

            for( int i = 0; i < task2_N_counter; i++)
            {
               xQueueSend(xQueue, &i, portMAX_DELAY);
               vTaskDelay(50 / portTICK_PERIOD_MS);

            }


            for( int i = 0; i < task2_N_counter; i++)
            {
                dd_led_on(LED_GREEN);
                vTaskDelay(300 / portTICK_PERIOD_MS);
                dd_led_off(LED_GREEN);  
                vTaskDelay(500 / portTICK_PERIOD_MS);
                printf("\r\nTask 2: LED GREEN blitzz\r\n");
            }
        }
    }
}


void task_3(void *pvParameters) 
{
  while (1)
  {
    int data;

    vTaskDelay(ONE_SECOND / portTICK_PERIOD_MS);

    while(xQueueReceive(xQueue, &data, 200 / portTICK_PERIOD_MS) == pdTRUE)
    {
        if(data == 0)
        {
            printf("\r\nTask 3: Buffer content: ");
        }
        printf("%d ,", data);
    }
    
  }
}

void run_idle_task_1() 
{
  
}