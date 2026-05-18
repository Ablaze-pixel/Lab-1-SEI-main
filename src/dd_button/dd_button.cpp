#include "dd_button.h"
#include <Arduino.h>

#define BUTTON_COUNT 3

// Vector cu pinii butoanelor (Analog pins folosiți ca Digital Input)
static const uint8_t button_pins[BUTTON_COUNT] = {A8, A9, A10};

void dd_button_setup(void)
{
    for (uint8_t i = 0; i < BUTTON_COUNT; i++)
    {
        // Folosim INPUT_PULLUP pentru a nu avea nevoie de rezistențe externe
        pinMode(button_pins[i], INPUT_PULLUP);
    }
}

uint8_t dd_button_pressed(uint8_t button_id)
{
    if (button_id < BUTTON_COUNT)
    {
        // Returnează 1 dacă este apăsat (LOW), 0 dacă este liber (HIGH)
        return (digitalRead(button_pins[button_id]) == LOW);
    }
    return 0;
}