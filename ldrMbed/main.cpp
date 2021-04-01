/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <cstdio>


// Declare
AnalogIn ldr1(A4);
AnalogIn ldr2(A5);
AnalogIn ldr3(A6);

I2CSlave slave(D4, D5);

int state = 0;
int ldr1T = 0;
int ldr2T = 0;
int ldr3T = 0;
int score = 0;
int cursor = -1;
char scoreChar[4];

void int2char(int pointer, int val){
    if(val==0){
        scoreChar[pointer] = '0';
    }else if(val==1){
        scoreChar[pointer] = '1';
    }else if(val==2){
        scoreChar[pointer] = '2';
    }else if(val==3){
        scoreChar[pointer] = '3';
    }else if(val==4){
        scoreChar[pointer] = '4';
    }else if(val==5){
        scoreChar[pointer] = '5';
    }else if(val==6){
        scoreChar[pointer] = '6';
    }else if(val==7){
        scoreChar[pointer] = '7';
    }else if(val==8){
        scoreChar[pointer] = '8';
    }else if(val==9){
        scoreChar[pointer] = '9';
    }
}

void scoreCounter(){
    score = 0;

    for(int i=0; i<6000; i++){
        int ldrVal1 = ldr1.read_u16();
        int ldrVal2 = ldr2.read_u16();
        int ldrVal3 = ldr3.read_u16();

        if(ldrVal1<=ldr1T||ldrVal2<=ldr2T||ldrVal3<=ldr3T){
            score++;
        }

        ThisThread::sleep_for(10ms);
    }
    printf("score : %d\n", score);

    for(int i=0; i<4; i++){
        int base = score/pow(10, 3-i);
        int val = score/base;
        int2char(i, val);
    }
    cursor = 0;
    state = 3;
}

void calibrate(){
    ThisThread::sleep_for(2000ms);

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
                    slave.write(scoreChar[cursor]);
                    cursor++;
                    printf("done\n");
                    if(cursor==4){
                        cursor = -1;
                        state = 0;
                    }
                }
                break;              
            case I2CSlave::WriteAddressed:
                slave.read(buf, sizeof(buf)-1);

                int num = atoi(buf);
                printf("Read: %d\n", num);
                state = num;
                cursor = -1;
                break;
        }

        if(state==1){
            //countscore
            printf("start count score\n");
            scoreCounter();
        }else if(state==2){
            printf("start calibrate\n");
            printf("%d %d %d\n", ldr1T, ldr2T, ldr3T);
            calibrate();
            printf("%d %d %d\n", ldr1T, ldr2T, ldr3T);
        }
    }
}
