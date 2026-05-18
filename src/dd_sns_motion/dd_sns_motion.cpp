#include "dd_sns_motion.h"
#include <Arduino.h>


#define SNS_PIN A11


void dd_sns_motion_setup (void)
{
    pinMode(SNS_PIN, INPUT);
}

 int dd_sns_motion_read()
{
    return digitalRead(SNS_PIN);
}




