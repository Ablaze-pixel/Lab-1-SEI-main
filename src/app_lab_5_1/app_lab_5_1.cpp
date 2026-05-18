#include "app_lab_5_1/app_lab_5_1.h"
#include "serial_stdio/serial_stdio.h"
#include "dd_DHT/dd_DHT.h"
#include "dd_relay/dd_relay.h"
#include "XFunctions_ADC---DAC/Filters.h"
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SETPOINT 24.0f
#define DEFAULT_HYSTERESIS 2.0f
#define SENSOR_INTERVAL_MS 2500
#define COMMAND_BUFFER_SIZE 32

static float g_setpoint = DEFAULT_SETPOINT;
static float g_hysteresis = DEFAULT_HYSTERESIS;
static bool g_relay_state = false;
static bool g_update_active = false;
static uint32_t g_last_sensor_ms = 0;
static char g_command_buffer[COMMAND_BUFFER_SIZE];
static size_t g_command_index = 0;

static void print_header(void)
{
    printf("Lab 5.1 - ON/OFF cu histereză pentru DHT22\r\n");
    printf("Comenzi: SET <temperatura>, UPDATE, STOP, STATUS\r\n");
    printf("Set point implicit: %.1f C, Bandă histereză: %.1f C\r\n", g_setpoint, g_hysteresis);
    printf("UPDATE pornește afișarea continuă a datelor\r\n");
    printf("STOP oprește afișarea fără a opri controlul\r\n\r\n");
}

static void print_status(float temperature, float humidity)
{
    float halfBand = g_hysteresis;
    float lowerThreshold = g_setpoint - halfBand;
    float upperThreshold = g_setpoint + halfBand;

    printf("Temp: %.1f C  Umiditate: %.1f%%  SetPoint: %.1f C  Bandă: %.1f-%.1f C  Releu: %s\r\n",
           temperature,
           humidity,
           g_setpoint,
           lowerThreshold,
           upperThreshold,
           g_relay_state ? "ON" : "OFF");
}

static void flush_command_buffer(void)
{
    g_command_index = 0;
    g_command_buffer[0] = '\0';
}

static void handle_command(const char *command)
{
    char tmp[COMMAND_BUFFER_SIZE];
    strncpy(tmp, command, COMMAND_BUFFER_SIZE - 1);
    tmp[COMMAND_BUFFER_SIZE - 1] = '\0';

    for (size_t i = 0; tmp[i] != '\0'; i++) {
        tmp[i] = (char)toupper((unsigned char)tmp[i]);
    }

    if (strncmp(tmp, "SET ", 4) == 0) {
        float value = atof(tmp + 4);
        if (value != 0.0f || strchr(tmp + 4, '0') != NULL) {
            g_setpoint = value;
            printf("Set Point actualizat: %.1f C\r\n", g_setpoint);
        } else {
            printf("Format incorect. Folosiți: SET <temperatura>\r\n");
        }
    } else if (strcmp(tmp, "UPDATE") == 0) {
        g_update_active = true;
        printf("UPDATE activat. Afișare continuă pornită.\r\n");
    } else if (strcmp(tmp, "STOP") == 0) {
        g_update_active = false;
        printf("STOP activat. Afișare continuă oprită.\r\n");
    } else if (strcmp(tmp, "STATUS") == 0) {
        printf("Set Point: %.1f C  Histereză: %.1f C  Releu: %s\r\n",
               g_setpoint,
               g_hysteresis,
               g_relay_state ? "ON" : "OFF");
    } else {
        printf("Comandă necunoscută: %s\r\n", command);
        printf("Utilizați SET <temperatura>, UPDATE, STOP sau STATUS\r\n");
    }
}

void app_lab_5_1_setup(void)
{
    serial_stdio_setup();
    dd_dht_setup();
    dd_relay_setup();
    dd_relay_off();
    flush_command_buffer();
    print_header();
}

static void process_serial(void)
{
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\r' || c == '\n') {
            if (g_command_index > 0) {
                g_command_buffer[g_command_index] = '\0';
                handle_command(g_command_buffer);
                flush_command_buffer();
            }
        } else if (g_command_index < COMMAND_BUFFER_SIZE - 1) {
            g_command_buffer[g_command_index++] = c;
        }
    }
}

void app_lab_5_1_run(void)
{
    process_serial();

    uint32_t now = millis();
    if ((now - g_last_sensor_ms) >= SENSOR_INTERVAL_MS) {
        float temperature = 0.0f;
        float humidity = 0.0f;
        if (dd_dht_read(&temperature, &humidity)) {
            bool desiredState = filter_onOffHysteresis(temperature, g_setpoint, g_hysteresis, g_relay_state);
            if (desiredState) {
                dd_relay_on();
            } else {
                dd_relay_off();
            }
            g_relay_state = desiredState;

            if (g_update_active) {
                print_status(temperature, humidity);
            }
        } else if (g_update_active) {
            printf("Eroare citire DHT22 sau interval prea scurt.\r\n");
        }
        g_last_sensor_ms = now;
    }
}
