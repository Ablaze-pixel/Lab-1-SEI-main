#include "keypad_stdio.h"
#include <stdio.h>
#include <Keypad.h>
#include <Arduino.h>

#define KEYPAD_RAPEATE_DELAY 100
// Tastatura 4x4
const byte ROWS = 4;
const byte COLS = 4;

char HexaKeys[ROWS][COLS] = {
    {'1','2','3','A'},
    {'4','5','6','B'},
    {'7','8','9','C'},
    {'*','0','#','D'}
};

byte rowPins[ROWS] = {3,2,1,0};// Pinurile pentru rânduri
byte colPins[COLS] = {7,6,5,4};// Pinurile pentru coloane

Keypad costumKeypad = Keypad(makeKeymap(HexaKeys), rowPins, colPins, ROWS, COLS);

void keypad_stdio_setup() {
    FILE* my_stream = fdevopen (NULL, keypad_stdio_getKey);
    stdin = my_stream;
}

int keypad_stdio_getKey(FILE* stream) {

    char key;
    do 
    {
        key = costumKeypad.getKey();
        
    } while (key == NO_KEY);
        
    delay(KEYPAD_RAPEATE_DELAY);

    return key;
}