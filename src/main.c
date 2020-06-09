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

#include "led.h"

void blinky() {
    while (1) {
        SetLed(LED1, LED_ON);
        k_msleep(1000);
        SetLed(LED1, LED_OFF);
        k_msleep(1000);
    }
}
struct k_thread blinky_thread;

//uint8_t __aligned(8) blinky_buffer[4096];
//k_thread_stack_t* blinky_stack = &blinky_buffer[2048];

//uint8_t __aligned(8) blinky_stack[4096];

//k_thread_stack_t __aligned(8) blinky_stack[4096]; //does not work


// The following works because of the keyword z_thread_stack_element 
// scripts/gen_kobject_list.py parses the generated elf file for that keyword
// and adds it to the generated gperf hash  

//struct z_thread_stack_element __aligned(8) blinky_stack[4096];

//struct z_thread_stack_element __attribute__((__aligned__(4096))) blinky_stack[4096];

//K_THREAD_STACK_DEFINE(blinky_stack, 4096);

//struct z_thread_stack_element __attribute__((__aligned__(((1 << (31 - __builtin_clz(4096))) < 4096 ? 1 << (31 - __builtin_clz(4096) + 1) : 1 << (31 - __builtin_clz(4096)))))) blinky_stack[((1 << (31 - __builtin_clz(4096))) < 4096 ? 1 << (31 - __builtin_clz(4096) + 1) : 1 << (31 - __builtin_clz(4096)))];

int main ( void ) {
	k_thread_system_pool_assign(k_current_get());	
	// Does not work
	//struct z_thread_stack_element *blinky_stack = k_malloc(4096);
	//blinky_stack = Z_STACK_PTR_ALIGN(blinky_stack);

//	struct k_thread* blinky_thread =  k_object_alloc(K_OBJ_THREAD);
//	if(blinky_thread == NULL){
//		printk("Could not allocate blinky_thread");
//		return -1;
//	}

	printk("size of struct z_stack_data: %d\n",sizeof(struct z_stack_data));	

	struct z_thread_stack_element* blinky_stack = k_object_alloc(K_OBJ_THREAD_STACK_ELEMENT,4096);
	
	if(blinky_stack == NULL){
		printk("Could not allocate blinky_stack");
		return -1;
	}


	

	//printk("Aligned to %d\n",((1 << (31 - __builtin_clz(4096))) < 4096 ? 1 << (31 - __builtin_clz(4096) + 1) : 1 << (31 - __builtin_clz(4096))));
	//printk("Blinky stack size %d\n",((1 << (31 - __builtin_clz(4096))) < 4096 ? 1 << (31 - __builtin_clz(4096) + 1) : 1 << (31 - __builtin_clz(4096)))); 
    
	//printk("Alignment of stack %d\n",__alignof__(blinky_stack));
	//printk("Size of stack %d\n",sizeof(blinky_stack));

	//for(uint32_t i = 0; i < sizeof(blinky_stack); i++){
	//	blinky_stack[i] = i;
	//}

	SetLed(LED0, LED_ON);
    k_thread_create(&blinky_thread,
                    blinky_stack,
                    4096,
                    blinky,
                    NULL, NULL, NULL,
                    3, K_USER, K_NO_WAIT);

	while (1) {
    	k_msleep(1000);
	}
    return 0;
}

