#ifndef DD_RELAY_H
#define DD_RELAY_H

#include <stdint.h>
#include <stdbool.h>

void dd_relay_setup(void);
void dd_relay_on(void);
void dd_relay_off(void);
bool dd_relay_is_on(void);

#endif
