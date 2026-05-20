#include "app_lab_3_1/app_lab_3_1.h"
#include "dd_sns_motion/dd_sns_motion.h"
#include "serial_stdio/serial_stdio.h"
#include "Arduino_FreeRTOS.h"

#define SENSOR_TASK_RECURRENCE_MS 250
#define SENSOR_TASK_OFFSET_MS 50
#define STATUS_TASK_RECURRENCE_MS 500

static volatile int g_motion_state = 0; //variabila pentru stocarea starii curente a senzorului de miscare (0 = fara miscare, 1 = miscare detectata)
static volatile uint32_t g_sample_count = 0; //variabila pentru numararea esantioanelor preluate de la senzor
static volatile uint32_t g_motion_count = 0; //variabila pentru numararea evenimentelor de miscare detectata

static void motion_task(void *pvParameters);
static void IDLE_status_task(void *pvParameters);

void app_lab_3_1_setup(void)
{
    serial_stdio_setup();
    dd_sns_motion_setup();

    printf("Lab 3.1 - Motion sensor acquisition\r\n");
    printf("Starting sensor acquisition and status tasks...\r\n\r\n");

    xTaskCreate(motion_task, "Motion Task", 256, NULL, 2, NULL);
    xTaskCreate(IDLE_status_task, "Status Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();
}

void app_lab_3_1_run(void)
{

}

void motion_task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    vTaskDelay(SENSOR_TASK_OFFSET_MS / portTICK_PERIOD_MS);

    while (1)
    {
        int motion = dd_sns_motion_read(); //citirea starii senzorului de miscare (0 sau 1)
        if (motion && !g_motion_state)
        {
            g_motion_count++;
        }

        g_motion_state = motion; //actualizarea starii curente a senzorului
        g_sample_count++; 

        vTaskDelayUntil(&xLastWakeTime, SENSOR_TASK_RECURRENCE_MS / portTICK_PERIOD_MS);
    }
}

void IDLE_status_task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        printf("=== Lab 3.1 status ===\r\n");
        printf("Samples acquired: %u\r\n", (unsigned int)g_sample_count);
        printf("Motion state: %s\r\n", g_motion_state ? "DETECTED" : "NONE");
        printf("Motion events: %u\r\n", (unsigned int)g_motion_count);
        printf("Next update in %d ms\r\n\r\n", STATUS_TASK_RECURRENCE_MS);

        vTaskDelayUntil(&xLastWakeTime, STATUS_TASK_RECURRENCE_MS / portTICK_PERIOD_MS);
    }
}
