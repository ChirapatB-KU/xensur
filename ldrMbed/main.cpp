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


int main()
{
    while (true) {
        printf("%d %d %d\n", ldr1.read_u16(), ldr2.read_u16(), ldr3.read_u16());
        ThisThread::sleep_for(500ms);
    }
}