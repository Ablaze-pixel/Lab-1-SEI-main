#include "app_lab_5_2/app_lab_5_2.h"
#include "serial_stdio/serial_stdio.h"
#include "dd_DHT/dd_DHT.h"
#include "dd_L298/dd_L298.h"
#include "XFunctions_ADC---DAC/Filters.h"
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_SETPOINT 26.0f
#define DEFAULT_KP 2.0f
#define DEFAULT_KI 0.5f
#define DEFAULT_KD 0.1f
#define SENSOR_INTERVAL_MS 2500
#define COMMAND_BUFFER_SIZE 48
#define PWM_MAX 255
#define PWM_MIN 0

static float g_setpoint = DEFAULT_SETPOINT;
static float g_kp = DEFAULT_KP;
static float g_ki = DEFAULT_KI;
static float g_kd = DEFAULT_KD;
static float g_pid_output = 0.0f;
static bool g_update_active = false;
static uint32_t g_last_sensor_ms = 0;
static uint32_t g_last_pid_ms = 0;
static char g_command_buffer[COMMAND_BUFFER_SIZE];
static size_t g_command_index = 0;

static void print_header(void)
{
    printf("Lab 5.2 - PID Control cu DHT22\r\n");
    printf("Comenzi: SET <temp>, KP <val>, KI <val>, KD <val>, UPDATE, STOP, STATUS\r\n");
    printf("Exemplu: SET 28.0  KP 2.5  KI 0.3  KD 0.05\r\n");
    printf("UPDATE porneste afisare continua (Serial Plotter)\r\n\r\n");
}

static void print_status(float temperature, float humidity)
{
    int16_t pwm_val = (int16_t)((g_pid_output * PWM_MAX) / 100.0f);

    printf("SetPoint: %.1f  Temp: %.1f  Humidity: %.1f%%  PID_Output: %.1f%%  PWM: %d\r\n",
           g_setpoint,
           temperature,
           humidity,
           g_pid_output,
           pwm_val);
}

static void print_plotter_format(float temperature, float humidity)
{
    int16_t pwm_val = (int16_t)((g_pid_output * PWM_MAX) / 100.0f);
    printf("%.1f,%.1f,%.1f,%d\r\n", g_setpoint, temperature, g_pid_output, pwm_val);
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
            filter_pidReset();
            printf("Set Point: %.1f C\r\n", g_setpoint);
        } else {
            printf("Format: SET <temperatura>\r\n");
        }
    } else if (strncmp(tmp, "KP ", 3) == 0) {
        float value = atof(tmp + 3);
        if (value >= 0.0f) {
            g_kp = value;
            filter_pidSetGains(g_kp, g_ki, g_kd);
            printf("Kp = %.2f\r\n", g_kp);
        } else {
            printf("Kp trebuie pozitiv\r\n");
        }
    } else if (strncmp(tmp, "KI ", 3) == 0) {
        float value = atof(tmp + 3);
        if (value >= 0.0f) {
            g_ki = value;
            filter_pidSetGains(g_kp, g_ki, g_kd);
            printf("Ki = %.2f\r\n", g_ki);
        } else {
            printf("Ki trebuie pozitiv\r\n");
        }
    } else if (strncmp(tmp, "KD ", 3) == 0) {
        float value = atof(tmp + 3);
        if (value >= 0.0f) {
            g_kd = value;
            filter_pidSetGains(g_kp, g_ki, g_kd);
            printf("Kd = %.2f\r\n", g_kd);
        } else {
            printf("Kd trebuie pozitiv\r\n");
        }
    } else if (strcmp(tmp, "UPDATE") == 0) {
        g_update_active = true;
        printf("UPDATE: ON (Serial Plotter mode)\r\n");
        printf("SetPoint,Temp,PID_Output,PWM\r\n");
    } else if (strcmp(tmp, "STOP") == 0) {
        g_update_active = false;
        printf("UPDATE: OFF\r\n");
    } else if (strcmp(tmp, "STATUS") == 0) {
        printf("SetPoint: %.1f C, Kp: %.2f, Ki: %.2f, Kd: %.2f, PID_Out: %.1f%%\r\n",
               g_setpoint, g_kp, g_ki, g_kd, g_pid_output);
    } else {
        printf("Comanda necunoscuta: %s\r\n", command);
    }
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

static void apply_pid_output(void)
{
    int16_t pwm_val = (int16_t)((g_pid_output * PWM_MAX) / 100.0f);

    if (pwm_val > 0) {
        dd_l298_set_power(g_pid_output);
    } else {
        dd_l298_stop();
    }
}

void app_lab_5_2_setup(void)
{
    serial_stdio_setup();
    dd_dht_setup();
    dd_l298_setup();
    flush_command_buffer();

    filter_pidSetGains(g_kp, g_ki, g_kd);
    filter_pidReset();

    print_header();
}

void app_lab_5_2_run(void)
{
    process_serial();

    uint32_t now = millis();
    if ((now - g_last_sensor_ms) >= SENSOR_INTERVAL_MS) {
        float temperature = 0.0f;
        float humidity = 0.0f;
        if (dd_dht_read(&temperature, &humidity)) {
            uint32_t dt_ms = now - g_last_pid_ms;
            g_pid_output = filter_pidCompute(g_setpoint, temperature, (float)dt_ms);
            apply_pid_output();

            if (g_update_active) {
                print_plotter_format(temperature, humidity);
            } else {
                print_status(temperature, humidity);
            }
        }
        g_last_sensor_ms = now;
        g_last_pid_ms = now;
    }
}
