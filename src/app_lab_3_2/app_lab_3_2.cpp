#include "app_lab_3_2/app_lab_3_2.h"
#include "serial_stdio/serial_stdio.h"
#include "Arduino_FreeRTOS.h"
#include "dd_sns_motion/dd_sns_motion.h"

#define SENSOR_TASK_RECURRENCE_MS 250
#define SENSOR_TASK_OFFSET_MS 50
#define STATUS_TASK_RECURRENCE_MS 500

static volatile int g_motion_state = 0;
static volatile uint32_t g_sample_count = 0;
static volatile uint32_t g_motion_count = 0;

static void motion_task(void *pvParameters);
static void IDLE_status_task(void *pvParameters);

void app_lab_3_2_setup()
{
    serial_stdio_setup();
    dd_sns_motion_setup();

    printf("Lab 3.2 - Motion sensor acquisition\r\n");
    printf("Starting sensor acquisition and status tasks...\r\n\r\n");

    xTaskCreate(motion_task, "Motion Task", 256, NULL, 2, NULL);
    xTaskCreate(IDLE_status_task, "Status Task", 256, NULL, 1, NULL);

    vTaskStartScheduler();
}

void app_lab_3_2_run()
{
}

void motion_task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    vTaskDelay(SENSOR_TASK_OFFSET_MS / portTICK_PERIOD_MS);

    while (1)
    {
        int motion = dd_sns_motion_read();
        if (motion && !g_motion_state)
        {
            g_motion_count++;
        }

        g_motion_state = motion;
        g_sample_count++;

        vTaskDelayUntil(&xLastWakeTime, SENSOR_TASK_RECURRENCE_MS / portTICK_PERIOD_MS);
    }
}

void IDLE_status_task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while (1)
    {
        printf("=== Lab 3.2 status ===\r\n");
        printf("Samples acquired: %u\r\n", (unsigned int)g_sample_count);
        printf("Motion state: %s\r\n", g_motion_state ? "DETECTED" : "NONE");
        printf("Motion events: %u\r\n", (unsigned int)g_motion_count);
        printf("Next update in %d ms\r\n\r\n", STATUS_TASK_RECURRENCE_MS);

        vTaskDelayUntil(&xLastWakeTime, STATUS_TASK_RECURRENCE_MS / portTICK_PERIOD_MS);
    }
}
