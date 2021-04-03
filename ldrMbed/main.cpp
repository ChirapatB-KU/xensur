/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>
#include <sstream>
#include <string>


// Declare
AnalogIn ldr1(A2);
AnalogIn ldr2(A5);
AnalogIn ldr3(A6);

I2CSlave slave(D4, D5);

int state = 0;
int ldr1T = 0;
int ldr2T = 0;
int ldr3T = 0;
int score = 0;
int cursor = 0;
std::string scoreString = "";


void scoreCounter(){
    score = 0;

    for(int i=0; i<6000; i++){
        int ldrVal1 = ldr1.read_u16();
        int ldrVal2 = ldr2.read_u16();
        int ldrVal3 = ldr3.read_u16();

        if(ldrVal1<=ldr1T||ldrVal2<=ldr2T||ldrVal3<=ldr3T){
            score++;
        }

        ThisThread::sleep_for(10);
    }
    printf("score : %d\n", score);

    scoreString = "";
    scoreString = std::to_string(score);
    for(int i=scoreString.length(); i<4; i++){
        scoreString = "0"+scoreString;
    }    
    state = 3;
}

void calibrate(){
    ThisThread::sleep_for(2000);

    ldr1T = ldr1.read_u16();
    ldr2T = ldr2.read_u16();
    ldr3T = ldr3.read_u16();

    ldr1T += -8000; 
    ldr2T += -8000;
    ldr3T += -8000;

    state = 0;
}

int main()
{
    char buf[20];
    printf("LDR ready...\n");
    slave.address(0xA0);

    while (true) {
        int i = slave.receive();
        for(int i=0; i<sizeof(buf); i++){
            buf[i] = 0;
        }
        switch(i){
            case I2CSlave::ReadAddressed:
                if(state==3){
                    printf("sending\n");
                    slave.write(scoreString.c_str(), 4);
                    printf("done\n");
                    state = 0;
                    ThisThread::sleep_for(1000);
                }
                break;              
            case I2CSlave::WriteAddressed:
                slave.read(buf, sizeof(buf)-1);

                int num = atoi(buf);
                printf("Read: %d\n", num);
                state = num;
                break;
        }

        if(state==1){
            //countscore
            printf("start count score\n");
            scoreCounter();
            state = 3;
        }else if(state==2){
            printf("start calibrate\n");
            printf("%d %d %d\n", ldr1T, ldr2T, ldr3T);
            calibrate();
            printf("%d %d %d\n", ldr1T, ldr2T, ldr3T);
        }
    }
}
