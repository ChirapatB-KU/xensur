/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "Servo.h"


// Declare devices' pin
Servo servo1(PB_1);
Servo servo2(PA_8);
Servo servo3(PA_11);


//  Declare variables
bool status = true;


int main()
{
    servo1 = 0;
    servo2 = 0;
    servo3 = 0;

    if(status){
        //init
        servo1 = 1;
        ThisThread::sleep_for(300ms);
        servo2 = 1;
        ThisThread::sleep_for(300ms);
        servo3 = 1;
        ThisThread::sleep_for(300ms);
        for(int i=0; i<5; i++){
            servo1 = !servo1;
            servo2 = !servo2;
            servo3 = !servo3;
            ThisThread::sleep_for(500ms);
        }
    }
}
