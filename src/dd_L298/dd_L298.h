#ifndef DD_L298_H
#define DD_L298_H

#include <stdint.h>

void    dd_l298_setup(void);
void    dd_l298_set_power(int16_t power);
void    dd_l298_stop(void);
void    dd_l298_max(void);
void    dd_l298_inc(void);
void    dd_l298_dec(void);
int16_t dd_l298_get_power(void);

#endif // DD_L298_H