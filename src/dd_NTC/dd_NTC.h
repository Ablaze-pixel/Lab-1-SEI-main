#ifndef DD_NTC_H
#define DD_NTC_H

#include <stdint.h>

void dd_ntc_setup(void);
uint16_t dd_ntc_read_raw(void);
float dd_ntc_read_voltage(void);
float dd_ntc_read_temperature_celsius(void);

#endif
