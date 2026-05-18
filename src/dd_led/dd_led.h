#ifndef DD_LED_H
#define DD_LED_H

#include <stdint.h>

void dd_led_setup(void);
void dd_led_on(uint8_t led_id);
void dd_led_off(uint8_t led_id);
void dd_led_toggle(uint8_t led_id);
bool dd_led_read_state(uint8_t led_id);

#endif