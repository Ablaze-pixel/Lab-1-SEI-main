#include "dd_l298/dd_l298.h"
#include <Arduino.h>
#include <stdlib.h>

// Definiții pini
#define L298_ENA  2  // Pin PWM analog
#define L298_IN1  15 
#define L298_IN2  16 

static int16_t current_power = 0;

void dd_l298_setup(void) {
    pinMode(L298_ENA, OUTPUT);
    pinMode(L298_IN1, OUTPUT);
    pinMode(L298_IN2, OUTPUT);
    dd_l298_stop(); // Pornim cu motorul oprit
}

void dd_l298_set_power(int16_t power) {
    if (power > 100)  power = 100;
    if (power < -100) power = -100;
    
    current_power = power;
    uint8_t pwm_val = (uint8_t)((abs(current_power) * 255) / 100);

    if (current_power > 0) {
        digitalWrite(L298_IN1, HIGH);
        digitalWrite(L298_IN2, LOW);
        analogWrite(L298_ENA, pwm_val); // Înainte
    } else if (current_power < 0) {
        digitalWrite(L298_IN1, LOW);
        digitalWrite(L298_IN2, HIGH);
        analogWrite(L298_ENA, pwm_val); // Înapoi
    } else {
        digitalWrite(L298_IN1, LOW);
        digitalWrite(L298_IN2, LOW);
        analogWrite(L298_ENA, 0);       // Stop
    }
}

void dd_l298_stop(void) {
    dd_l298_set_power(0);
}

void dd_l298_max(void) {
    if (current_power >= 0) {
        dd_l298_set_power(100);
    } else {
        dd_l298_set_power(-100);
    }
}

void dd_l298_inc(void) {
    int16_t new_power = current_power + 10;
    if (new_power > 100) new_power = 100;
    dd_l298_set_power(new_power);
}

void dd_l298_dec(void) {
    // Decrementarea tinde întotdeauna spre 0%
    if (current_power > 0) {
        int16_t new_power = current_power - 10;
        if (new_power < 0) new_power = 0;
        dd_l298_set_power(new_power);
    } 
    else if (current_power < 0) {
        int16_t new_power = current_power + 10;
        if (new_power > 0) new_power = 0;
        dd_l298_set_power(new_power);
    }
}

int16_t dd_l298_get_power(void) {
    return current_power;
}