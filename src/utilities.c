/**
 ******************************************************************************
 * @file    utilities.c
 * @brief   Contains helper code
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

#include <utilities.h>

__attribute__((optimize("O0")))
void Hault(sContextStateFrame *frame) {
    // If and only if a debugger is attached, execute a breakpoint
    // instruction so we can take a look at what triggered the fault
#if (__ARM_ARCH == 7)
    do {
        if ((*(volatile uint32_t *)0xE000EDF0) & (1 << 0)) {
            __asm("bkpt 1");
        }
    } while (0);
#else
#error "Devices with ARM arch ARM-v7M supported"
#endif
}

void FaultHandler() {
    asm volatile("tst lr, #4      ;"
                 "ite eq          ;"
                 "mrseq r0, msp   ;"
                 "mrsne r0, psp   ;"
                 "b Hault         ;"
                );
    while(1);
}

