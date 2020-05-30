/**
 ******************************************************************************
 * @file    led.c
 * @brief   Contains APIs for setting state of the LED
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


#ifdef RKERNEL

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>

#include "utilities.h"
#include "led.h"

#define LED0_NODE DT_ALIAS(led0)
#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
    #define LED0_DEV	DT_GPIO_LABEL(LED0_NODE, gpios)
    #define LED0_PIN	DT_GPIO_PIN(LED0_NODE, gpios)
    #if DT_PHA_HAS_CELL(LED0_NODE, gpios, flags)
        #define LED0_FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
    #endif

    #ifndef LED0_FLAGS
        #define LED0_FLAGS	0
    #endif

#else
    #error "Unsupported board: led0 is not aliased in device tree."
#endif

#define LED1_NODE DT_ALIAS(led1)
#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
    #define LED1_DEV	DT_GPIO_LABEL(LED1_NODE, gpios)
    #define LED1_PIN	DT_GPIO_PIN(LED1_NODE, gpios)
    #if DT_PHA_HAS_CELL(LED1_NODE, gpios, flags)
        #define LED1_FLAGS	DT_GPIO_FLAGS(LED1_NODE, gpios)
    #endif

    #ifndef LED1_FLAGS
        #define LED1_FLAGS	0
    #endif
#else
    #error "Unsupported board: led1 is not aliased in device tree."
#endif


struct device *dev;

int8_t initLeds(void) {
    // Initialize GPIO and LED
    int ret;
    dev = device_get_binding(LED0_DEV);
    if (dev == NULL) {
        return -1;
    }
    ret = gpio_pin_configure(dev, LED0_PIN, GPIO_OUTPUT_INACTIVE | LED0_FLAGS);
    if (ret < 0) {
        return -1;
    }

    dev = device_get_binding(LED1_DEV);
    if (dev == NULL) {
        return -1;
    }
    ret = gpio_pin_configure(dev, LED1_PIN, GPIO_OUTPUT_INACTIVE | LED1_FLAGS);
    if (ret < 0) {
        return -1;
    }
    return 0;
}

int8_t SetLed(uint8_t Led_Num, uint8_t Led_State) {
    switch(Led_Num) {
        case 0:
            dev = device_get_binding(LED0_DEV);
            if (dev == NULL) {
                return -1;
            }
            gpio_pin_set(dev, LED0_PIN, (int)Led_State);
            break;
        case 1:
            dev = device_get_binding(LED1_DEV);
            if (dev == NULL) {
                return -1;
            }
            gpio_pin_set(dev, LED1_PIN, (int)Led_State);
            break;
		default:
			return -1;
    }
    return 0;
}

#endif /* RKERNEL */
