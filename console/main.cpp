/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "Servo.h"
#include <cstdio>
#include <cstdlib>


// Declare devices' pin
Servo servo1(PB_1);
Servo servo2(PA_8);
Servo servo3(PA_11);


//  Declare variables
bool status = true;
int time_left = 0;
int sel[100];
Thread t1, t2, t3;


void countdown(){
    while(time_left>0){
        printf("%d\n", time_left);
        time_left = time_left-1;
        ThisThread::sleep_for(1s);
    }
}

void turnServo(int i, int j){
    if(i==1){
        servo1 = j;
    }else if(i==2){
        servo2 = j;
    }else if(i==3){
        servo3 = j;
    }
}

void hideAllServo(){
    servo1 = 0;
    servo2 = 0;
    servo3 = 0;
}

void randomSel(){
    for(int i=0; i<100; i++){
        sel[i] = (rand()%3)+1;
    }
}

void game(){
    //init
    hideAllServo();
    int i = 0;

    //time 59-50s
    ThisThread::sleep_for(2s);
    turnServo(sel[i], 1);
    ThisThread::sleep_for(2s);
    turnServo(sel[i], 0);
    i++;
    ThisThread::sleep_for(2s);
    turnServo(sel[i], 1);
    ThisThread::sleep_for(2s);
    turnServo(sel[i], 0);
    i++;
    ThisThread::sleep_for(2s);

    //time 49-30s
    hideAllServo();
    for(int j=0; j<10; j++){
        turnServo(sel[i], 1);
        ThisThread::sleep_for(1500ms);
        turnServo(sel[i], 0);
        i++;
        ThisThread::sleep_for(500ms);
    }

    //time 29-5s
    hideAllServo();
    for(int j=0; j<5; j++){
        ThisThread::sleep_for(1500ms);
        turnServo(sel[i], 1);
        turnServo(sel[i+1], 1);
        ThisThread::sleep_for(2s);
        turnServo(sel[i], 0);
        turnServo(sel[i+1], 0);
        i+=2;
        ThisThread::sleep_for(1500ms);
    }

    //time 4-0s
    hideAllServo();
    turnServo(sel[i], 1);
    ThisThread::sleep_for(5s);
    turnServo(sel[i], 0);
}

int main()
{
    servo1 = 0;
    servo2 = 0;
    servo3 = 0;

    srand(time(0));

    if(status){
        //init
        t1.start(callback(randomSel));

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

        time_left = 60;

        t2.start(callback(countdown));
        t3.start(callback(game));
        wait_us((time_left+10)*1000000);
    }

    printf("done\n");
}
