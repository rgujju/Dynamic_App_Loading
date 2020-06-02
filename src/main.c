/**
 ******************************************************************************
 * @file    main.c
 * @brief   Main file
 *          Load the app and also turn on a LED
 ******************************************************************************
 * @attention
 *
 * LICENSE
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Rohit Gujarathi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#include "utilities.h"
#include "sys_module/led.h"

void blinky() {
    while (1) {
        SetLed(LED1, LED_ON);
        k_msleep(1000);
        SetLed(LED1, LED_OFF);
        k_msleep(1000);
    }
}
struct k_thread blinky_thread;
k_thread_stack_t blinky_stack[4096];

int main ( void ) {
    initLeds();
    SetLed(LED0, LED_ON);
    k_thread_create(&blinky_thread,
                    blinky_stack,
                    sizeof(blinky_stack),
                    blinky,
                    NULL, NULL, NULL,
                    3, K_USER, K_NO_WAIT);
    //while (1) {
    //}
    return 0;
}

