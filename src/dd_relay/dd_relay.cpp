#include "dd_relay.h"
#include <Arduino.h>

#define RELAY_PIN 8

static bool relay_state = false;

void dd_relay_setup(void)
{
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    relay_state = false;
}

void dd_relay_on(void)
{
    digitalWrite(RELAY_PIN, HIGH);
    relay_state = true;
}

void dd_relay_off(void)
{
    digitalWrite(RELAY_PIN, LOW);
    relay_state = false;
}

bool dd_relay_is_on(void)
{
    return relay_state;
}
