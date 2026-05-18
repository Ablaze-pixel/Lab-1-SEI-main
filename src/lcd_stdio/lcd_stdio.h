#ifndef LCD_STDIO_H
#define LCD_STDIO_H
#include <stdio.h>

void lcd_stdio_setup();
int lcd_stdio_print(char c, FILE* stream);

#endif