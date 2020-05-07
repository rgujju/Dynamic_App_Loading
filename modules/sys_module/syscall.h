/**
 ******************************************************************************
 * @file    syscall.h
 * @brief   Contains APIs for system calls from the APP
 ******************************************************************************
 * @attention
 *
 * LICENSE
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Rohit Gujarathi rgujju.github.io
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


#pragma once

#include "sys_module/led.h"

/**
    @brief  This struct contains APIs for calling system functions
            It is basically a jump table. 
            Depending on weather the KERNEL is built or the libs for the app,
            different sys_struct definitions are compiled.
            For Kernel:
                The complete struct with each member is compiled
            For userlib for the app:
                Only a 'const sys_struct* sys' pointer is defined. The address
                of this pointer is made to point to the actual sys_struct in the
                kernel at load time in the LoadApp API
*/
typedef struct {
    int8_t (*SetLed)(uint8_t Led_Num, uint8_t Led_State);
}sys_struct;

#define DUMMY_SYS_ADDR 0xA5A5A5A5

#ifdef RKERNEL
    extern sys_struct sys;
#else
	extern const  sys_struct* sys;
#endif

