#include "dd_led.h"
#include <Arduino.h>

#define LED_COUNT 2

// Vector cu pinii LED-urilor
static const uint8_t led_pins[LED_COUNT] = {13, 12};

void dd_led_setup(void)
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        pinMode(led_pins[i], OUTPUT);
        digitalWrite(led_pins[i], LOW);
    }
}

void dd_led_on(uint8_t led_id)
{
    if (led_id < LED_COUNT)
    {
        digitalWrite(led_pins[led_id], HIGH);
    }
}

void dd_led_off(uint8_t led_id)
{
    if (led_id < LED_COUNT)
    {
        digitalWrite(led_pins[led_id], LOW);
    }
}

void dd_led_toggle(uint8_t led_id)
{
    if (led_id < LED_COUNT)
    {
        digitalWrite(led_pins[led_id], !digitalRead(led_pins[led_id]));
    }
}

bool dd_led_read_state(uint8_t led_id)
{
    if (led_id < LED_COUNT)
    {
        return digitalRead(led_pins[led_id]) == HIGH;
    }
    return false;
}