#ifndef SERIAL_STDIO_H
#define SERIAL_STDIO_H
#include <Arduino.h>

void serial_stdio_setup();
int serial_put_char(char ch, FILE *stream);
int serial_get_char(FILE *stream);

#ifdef __cplusplus
extern "C" {
#endif
void my_printf(const char *format, ...);
#ifdef __cplusplus
}
#endif

// Redefine printf to use my_printf
#define printf my_printf

#endif