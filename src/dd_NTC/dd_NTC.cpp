#include "dd_NTC/dd_NTC.h"
#include <Arduino.h>
#include <math.h>

#define NTC_PIN A0
#define ADC_MAX 1023.0f
#define V_REF 5.0f
#define SERIES_RESISTOR 10000.0f
#define NTC_BETA 3950.0f
#define NTC_R25 10000.0f
#define T0_KELVIN 298.15f

void dd_ntc_setup(void)
{
    pinMode(NTC_PIN, INPUT);
}

uint16_t dd_ntc_read_raw(void)
{
    return analogRead(NTC_PIN);
}

float dd_ntc_read_voltage(void)
{
    return (dd_ntc_read_raw() * V_REF) / ADC_MAX;
}

float dd_ntc_read_temperature_celsius(void)
{
    uint16_t raw = dd_ntc_read_raw();

    if (raw == 0 || raw >= ADC_MAX)
    {
        return -273.15f;
    }

    float celsius = 1 / (log(1 / (ADC_MAX / raw - 1)) / NTC_BETA + 1.0 / T0_KELVIN) - 273.15;
    return celsius;

}
