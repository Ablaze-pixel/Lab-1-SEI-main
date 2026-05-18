#include "lcd_stdio.h" 
#include <LiquidCrystal_I2C.h>
#include <stdio.h>

char CLEAR_KEY[] = "#";
#define BACKLIGHT_PIN     20
#define lcdColums         16
#define lcdRows           2

LiquidCrystal_I2C lcd(0x27, lcdColums, lcdRows);

void lcd_stdio_setup()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  
  FILE* stream = fdevopen(lcd_stdio_print, NULL);

  stdout = stream;
}


int lcd_stdio_print(char c, FILE* stream)
{
  if (c == CLEAR_KEY[0])
  {

        lcd.clear(); 
        lcd.setCursor(0, 0);

  }
    else
    {
    lcd.print(c);
    }
    return 0;
    
}