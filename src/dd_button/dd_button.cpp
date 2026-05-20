#include "dd_button.h"
#include <Arduino.h>

#define BUTTON_COUNT 3
static const uint8_t button_pins[BUTTON_COUNT] = {A8, A9, A10};

void dd_button_setup(void) {
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        pinMode(button_pins[i], INPUT_PULLUP);
    }
}

uint8_t dd_button_pressed(uint8_t button_id) {
    if (button_id < BUTTON_COUNT) {
        return (digitalRead(button_pins[button_id]) == LOW);
    }
    return 0;
}

// Implementarea funcției care blochează re-declanșarea până la eliberare
uint8_t dd_button_read_and_wait(uint8_t button_id) {
    if (button_id >= BUTTON_COUNT) return 0;

    // Dacă butonul este apăsat (LOW)
    if (digitalRead(button_pins[button_id]) == LOW) {
        delay(40); // Debounce la apăsare pentru a ignora zgomotul mecanic
        
        // Așteptăm activ (buclă) cât timp utilizatorul ține ÎNCĂ degetul pe buton
        while (digitalRead(button_pins[button_id]) == LOW) {
            // Nu facem nimic, așteptăm să ridice degetul
        }
        
        delay(40); // Debounce la eliberare
        return 1;  // Returnăm 1 doar după ce butonul a fost complet eliberat!
    }
    
    return 0; // Nu s-a detectat nicio apăsare
}