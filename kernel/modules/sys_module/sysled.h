/**
 ******************************************************************************
 * @file    led.h
 * @brief   Contains APIs for setting state of LED
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

#ifndef _RATCH_LED_H_
#define _RATCH_LED_H_

#include <common.h>

/** LED 0 */
#define LED0 0
/** LED 1 */
#define LED1 1
/** LED_ON state */
#define LED_ON 1
/** LED_OFF state */
#define LED_OFF 0

/** @brief  Change state of LED to on/off
 * 
 *  @note   This API is compiled only in the KERNEL, not in the userlib
 *          To call this API, from the app. use 'SetLed(LED1, LED_ON);'
 *          it will generate a svc and jump to implementation function
 *          in the kernel
 *
 *  @param  Led_Num Which LED to turn on
 *
 *  @param  Led_State Set the state of LED to on (1) or off (0)
 *
 *  @retval 0 
 *
*/
__syscall int8_t SetLed(uint8_t Led_Num, uint8_t Led_State);

#include <sys_module/sysled_kernel.h>
#endif /* _RATCH_LED_H_ */
