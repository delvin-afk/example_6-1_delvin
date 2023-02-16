//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"
#include "code.h"
#include "smart_home_system.h"


#define DUTY_MIN 0.025
#define DUTY_MAX 0.125
#define PERIOD 0.02
PwmOut servo(PF_9);

void rotate90degrees();
void openGate();


void rotate90degrees()
{
    servo.period(PERIOD); // 20ms period
    servo.write(DUTY_MIN);
    delay(5000.0);
    servo.write((DUTY_MIN + DUTY_MAX) / 2); // rotates to half of the duty max which is 90 degrees
    delay(5000.0);   // delays by 5 seconds
        
}

void openGate(){

    rotate90degrees();
   

}