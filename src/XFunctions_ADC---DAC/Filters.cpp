#include "Filters.h"
#include <math.h>

// Implementarea unui filtru digital de tip "sare și piper" pentru eliminarea zgomotului impulsiv.
// Implementarea unui filtru digital de mediere ponderată pentru netezirea suplimentară a semnalului achiziționat.
// Conversia ADC-to-Voltage și Voltage-to-Parametru fizic conform specificațiilor senzorului ales.
// Aplicarea saturării pentru a limita valorile prelucrate în intervale valide.

float filter_saltAndPepperFilter(float newValue) {
    static float window[5]; // Dimensiunea ferestrei (5 eșantioane)
    static int index = 0;
    static int filled = 0;

    window[index] = newValue;
    index = (index + 1) % 5;

    if (filled < 5) {
        for (int i = filled; i < 5; i++) {
            window[i] = newValue;
        }
        filled++;
    }

    float sorted[5];
    for (int i = 0; i < 5; i++) {
        sorted[i] = window[i];
    }

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j < 5; j++) {
            if (sorted[i] > sorted[j]) {
                float temp = sorted[i];
                sorted[i] = sorted[j];
                sorted[j] = temp;
            }
        }
    }

    return sorted[2]; // Valoarea mediană
}

float filter_weightedAverageFilter(float currentVal, float alpha) {
    static float previousFiltered = 0.0f;
    static bool initialized = false;

    if (!initialized) {
        previousFiltered = currentVal;
        initialized = true;
    }

    if (alpha <= 0.0f) {
        alpha = 0.2f;
    } else if (alpha > 1.0f) {
        alpha = 1.0f;
    }

    float filtered = (alpha * currentVal) + (1.0f - alpha) * previousFiltered;
    previousFiltered = filtered;

    return filtered;
}

float filter_applySaturation(float value, float minVal, float maxVal) {
    if (value > maxVal) return maxVal;
    if (value < minVal) return minVal;
    return value;
}

bool filter_onOffHysteresis(float currentValue, float setPoint, float hysteresis, bool previousState) {
    if (hysteresis < 0.0f) {
        hysteresis = -hysteresis;
    }

    float halfBand = hysteresis * 0.5f;
    float lowerThreshold = setPoint - halfBand;
    float upperThreshold = setPoint + halfBand;

    if (currentValue <= lowerThreshold) {
        return true;
    }
    if (currentValue >= upperThreshold) {
        return false;
    }

    return previousState;
}

static float g_pid_kp = 1.0f;
static float g_pid_ki = 0.0f;
static float g_pid_kd = 0.0f;
static float g_pid_integral = 0.0f;
static float g_pid_prevError = 0.0f;
static bool g_pid_initialized = false;

#define PID_INTEGRAL_MAX 100.0f
#define PID_INTEGRAL_MIN -100.0f

void filter_pidSetGains(float kp, float ki, float kd)
{
    g_pid_kp = kp;
    g_pid_ki = ki;
    g_pid_kd = kd;
    g_pid_initialized = true;
}

float filter_pidCompute(float setPoint, float currentValue, float dt_ms)
{
    if (!g_pid_initialized) {
        return 0.0f;
    }

    float error = setPoint - currentValue;
    float dt_sec = dt_ms / 1000.0f;

    float proportional = g_pid_kp * error;

    g_pid_integral += g_pid_ki * error * dt_sec;
    if (g_pid_integral > PID_INTEGRAL_MAX) {
        g_pid_integral = PID_INTEGRAL_MAX;
    }
    if (g_pid_integral < PID_INTEGRAL_MIN) {
        g_pid_integral = PID_INTEGRAL_MIN;
    }

    float derivative = 0.0f;
    if (dt_sec > 0.0f) {
        derivative = g_pid_kd * (error - g_pid_prevError) / dt_sec;
    }
    g_pid_prevError = error;

    float output = proportional + g_pid_integral + derivative;

    return filter_applySaturation(output, -100.0f, 100.0f);
}

void filter_pidReset(void)
{
    g_pid_integral = 0.0f;
    g_pid_prevError = 0.0f;
}

float filter_adcToVoltage(float rawValue, float vRef, float adcResolution) {
    if (adcResolution <= 0.0f) {
        return 0.0f;
    }
    return (rawValue * vRef) / adcResolution;
}

float filter_linearSensorConversion(float voltage, float slope, float offset) {
    return (slope * voltage) + offset;
}

float filter_adc_ToPhysical(float rawValue, float vRef, float adcResolution, float slope, float offset) {
    float voltage = filter_adcToVoltage(rawValue, vRef, adcResolution);
    return filter_linearSensorConversion(voltage, slope, offset);
}

float filter_adc_VoltageToTemperature(float voltage, float vRef, float seriesResistor, float r25, float beta) {
    if (voltage <= 0.0f || voltage >= vRef || seriesResistor <= 0.0f || r25 <= 0.0f || beta <= 0.0f) {
        return -273.15f;
    }

    float resistance = seriesResistor * voltage / (vRef - voltage);
    float temperatureKelvin = 1.0f / ((1.0f / (273.15f + 25.0f)) + (1.0f / beta) * logf(resistance / r25));
    return temperatureKelvin - 273.15f;
}

