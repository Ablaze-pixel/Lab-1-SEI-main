#ifndef DD_BUTTON_H
#define DD_BUTTON_H

#include <stdint.h>

void dd_button_setup(void);
uint8_t dd_button_pressed(uint8_t button_id);

#endif