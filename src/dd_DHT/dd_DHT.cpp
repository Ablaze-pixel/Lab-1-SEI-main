#include "dd_DHT/dd_DHT.h"
#include <Arduino.h>
#include <string.h>

#define DD_DHT22_DATA_PIN 14
#define DD_DHT22_START_SIGNAL_MS 20
#define DD_DHT22_WAIT_TIMEOUT_US 100
#define DD_DHT22_MIN_READ_INTERVAL_MS 2000

static bool g_initialized = false;
static uint32_t g_last_read_ms = 0;

static bool dd_dht_wait_for_state(uint8_t expectedState, uint32_t timeoutUs, uint32_t *durationUs)
{
    uint32_t start = micros();
    while (digitalRead(DD_DHT22_DATA_PIN) == expectedState) {
        if ((micros() - start) >= timeoutUs) {
            return false;
        }
    }
    if (durationUs != NULL) {
        *durationUs = micros() - start;
    }
    return true;
}

static bool dd_dht_read_raw(uint8_t data[5])
{
    memset(data, 0, 5);

    pinMode(DD_DHT22_DATA_PIN, OUTPUT);
    digitalWrite(DD_DHT22_DATA_PIN, LOW);
    delay(DD_DHT22_START_SIGNAL_MS);

    digitalWrite(DD_DHT22_DATA_PIN, HIGH);
    delayMicroseconds(40);
    pinMode(DD_DHT22_DATA_PIN, INPUT_PULLUP);

    // Răspunsul senzorului DHT22: LOW ~80us, HIGH ~80us
    if (!dd_dht_wait_for_state(LOW, DD_DHT22_WAIT_TIMEOUT_US, NULL)) {
        return false;
    }
    if (!dd_dht_wait_for_state(HIGH, DD_DHT22_WAIT_TIMEOUT_US, NULL)) {
        return false;
    }
    if (!dd_dht_wait_for_state(LOW, DD_DHT22_WAIT_TIMEOUT_US, NULL)) {
        return false;
    }

    for (uint8_t i = 0; i < 40; i++) {
        // LOW pulse start
        if (!dd_dht_wait_for_state(LOW, 100, NULL)) {
            return false;
        }

        // HIGH pulse length encodes bit value
        uint32_t highDuration = 0;
        if (!dd_dht_wait_for_state(HIGH, 100, &highDuration)) {
            return false;
        }

        uint8_t byteIndex = i / 8;
        data[byteIndex] <<= 1;
        if (highDuration > 50) {
            data[byteIndex] |= 1;
        }
    }

    return true;
}

void dd_dht_setup(void)
{
    g_initialized = true;
    g_last_read_ms = 0;
    pinMode(DD_DHT22_DATA_PIN, INPUT_PULLUP);
}

bool dd_dht_read(float *temperature, float *humidity)
{
    if (!g_initialized || temperature == NULL || humidity == NULL) {
        return false;
    }

    if ((millis() - g_last_read_ms) < DD_DHT22_MIN_READ_INTERVAL_MS) {
        return false;
    }

    uint8_t raw[5];
    if (!dd_dht_read_raw(raw)) {
        return false;
    }

    uint8_t checksum = raw[0] + raw[1] + raw[2] + raw[3];
    if (checksum != raw[4]) {
        return false;
    }

    int16_t rawHumidity = (raw[0] << 8) | raw[1];
    int16_t rawTemperature = (raw[2] << 8) | raw[3];
    *humidity = rawHumidity / 10.0f;
    if (rawTemperature & 0x8000) {
        rawTemperature &= 0x7FFF;
        *temperature = -rawTemperature / 10.0f;
    } else {
        *temperature = rawTemperature / 10.0f;
    }

    g_last_read_ms = millis();
    return true;
}
