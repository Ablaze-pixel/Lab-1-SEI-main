#include "serial_stdio/serial_stdio.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>

void serial_stdio_setup()
{
    Serial.begin(9600);
    FILE *serial_stream = fdevopen(serial_put_char, serial_get_char);
    stdin = serial_stream;
    stdout = serial_stream;
}
    
int serial_put_char(char ch, FILE *stream)
{
   return Serial.write(ch);
}

int serial_get_char(FILE *stream)
{
    while (!Serial.available())
        ;
    return Serial.read();
}

// Custom printf that supports %f
void my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    const char *f_ptr = format;
    while (*f_ptr) {
        if (*f_ptr == '%') {
            f_ptr++;
            int precision = 2; // default
            if (*f_ptr == '.') {
                f_ptr++;
                precision = 0;
                while (*f_ptr >= '0' && *f_ptr <= '9') {
                    precision = precision * 10 + (*f_ptr - '0');
                    f_ptr++;
                }
            }
            if (*f_ptr == 'f') {
                float f = va_arg(args, double);
                char float_str[16];
                dtostrf(f, 0, precision, float_str);
                Serial.print(float_str);
                f_ptr++;
            } else if (*f_ptr == 'u') {
                unsigned int u = va_arg(args, unsigned int);
                Serial.print(u);
                f_ptr++;
            } else if (*f_ptr == 'd') {
                int d = va_arg(args, int);
                Serial.print(d);
                f_ptr++;
            } else if (*f_ptr == 's') {
                char *s = va_arg(args, char*);
                Serial.print(s);
                f_ptr++;
            } else if (*f_ptr == 'c') {
                char c = (char)va_arg(args, int);
                Serial.write(c);
                f_ptr++;
            } else {
                Serial.write('%');
                if (*f_ptr != '%') Serial.write(*f_ptr);
                f_ptr++;
            }
        } else {
            Serial.write(*f_ptr++);
        }
    }
    va_end(args);
}
