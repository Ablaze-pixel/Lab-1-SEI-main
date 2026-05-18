#ifndef KEYPAD_STDIO_H
#define KEYPAD_STDIO_H  
#include <stdio.h>
#include <Arduino.h>

void keypad_stdio_setup();
int keypad_stdio_getKey(FILE* stream);             

#endif