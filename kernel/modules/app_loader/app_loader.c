/**
 ******************************************************************************
 * @file    app_loader.c
 * @brief   Contains function for loading the app and setting up the freertos
 *          task
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

#include "zephyr.h"
#include "kernel.h"
#include "string.h" //TODO: Replace with a minimal libc implementation
#include "stdlib.h" //TODO: Replace with a minimal libc implementation
#include "app_loader/app_loader.h"

#include <logging/log.h>
// TODO: Change Log module debug level from build system
LOG_MODULE_REGISTER(app_loader, 4);

// TODO Add Tests:
// 2) Check name length =0,1,15,16,and greater, Check if it is 0s after name length
// 3) Check little endian/ big endian or whatever
// 4) malloc fails
// 5) app loading fails during task creation
// 6) tinf->bin should fail

// RGREMOVE
#define DUMMY_SYS_ADDR 0xA5A5A5A5
uint32_t sys = 0xdeadbeef;
//void blinky() {
//    while (1) {
//        SetLed(LED0, LED_ON);
//        k_msleep(1000);
//        SetLed(LED0, LED_OFF);
//        k_msleep(1000);
//    }
//}
/////////////

#define DEFAULT_STACK_SIZE 1024
K_THREAD_STACK_DEFINE(app_stack, DEFAULT_STACK_SIZE);
struct k_thread app_thread; //TODO: Dynamically allocate thread struct

int8_t LoadApp(const uint8_t* tinf_img) {

    tinf_t* tinf = (tinf_t*)tinf_img;
    // Check if it is actually an APP
    // Check the magic, ie start of the header contains 'TINF'
    uint32_t* data_base = tinf->bin+tinf->text_size;

    if(memcmp(tinf->magic, "TINF", 4) == 0) {

        // Valid TINF Format app
        LOG_DBG("Loading app: %s %hu.%hu", tinf->app_name, tinf->major_version, tinf->minor_version);
        LOG_DBG("App text size: %hu 32 bit word", tinf->text_size);
        LOG_DBG("App data size: %hu 32 bit word", tinf->data_size);
        LOG_DBG("App bss size: %hu 32 bit word", tinf->bss_size);
        LOG_DBG("App GOT entries: %ld", tinf->got_entries);

        // Allocate memory for data and bss section of the app on the heap
        uint32_t app_data_size = tinf->data_size+tinf->got_entries+tinf->bss_size;

        LOG_DBG("Allocating app memory of %ld bytes", app_data_size*4);
        // TODO: Add the size of the stack actually required by the app, currently hardcoded to DEFAULT_STACK_SIZE words, change in the task create API also
        // TODO: Cannot dynamically allocate thread stack due to limitation in zephyr
        // Zephyr does not have an aligned allocater
        //uint32_t* app_data_base = k_malloc((app_data_size+DEFAULT_STACK_SIZE)*4);
        uint32_t* app_data_base = app_stack;
        uint32_t app_stack_size = DEFAULT_STACK_SIZE;
        //if(app_data_base == NULL) {
        //    return APP_OOM;
        //}

        // app_stack_base is the address of the base of the stack used by
        // the rtos task
        uint32_t* app_stack_base = app_data_base+app_data_size;

        // app_got_base is the value which will get loaded to r9
        // This is address of the base of GOT which will actually be used
        // by the app for global data accesses
        // This value is passed as the parameter to the app_main. The app will
        // copy the this value from r0 to r9 register
        uint32_t* app_got_base = app_data_base + tinf->data_size;
        LOG_DBG("App data base: 0x%08X", app_data_base);

        // Layout in RAM:
        // low memory (eg: 0x200014a0)                          high memory (eg: 0x200014cc)
        // |<--------------------- app_data_size ---------------------->|
        // |<-- tinf->data_size -->|<-- got_entries -->|<-- bss_size -->|<-- stack_size -->|
        // +-----------------------+-------------------+----------------+------------------+
        // | .data                 | .got              | .bss           | APP STACK        |
        // +-----------------------+-------------------+----------------+------------------+
        // ^                       ^                                    ^                  Λ
        // app_data_base           app_got_base                         app_stack_base     |
        //                                                  stack starts pushing from here |
        // TODO: There is no APP STACK overflow protection right now since we cannot detect
        //       when the stack will start overwriting the bss section

        if(app_data_size != 0) {
            if(tinf->data_size > 0) {
                // Copy data section from flash to the RAM we allocated above
                memcpy(app_data_base, data_base, (tinf->data_size*4));
                LOG_DBG("Data at data section (flash): 0x%08X", *(uint32_t*)(tinf->bin+(tinf->text_size)));
                LOG_DBG("Data at data section (RAM): 0x%08X", *(uint32_t*)(app_data_base));

                // Replace the sys_struct address to the one on the mcu
                // The start of the app_data_base will point to start of the data section
                // This is where the sys_struct address was kept by the linker script
                //*app_data_base = (uintptr_t)&sys;
                //LOG_DBG("App sys_struct: 0x%08X", *app_data_base);
            }

            // If there is any global data of the app then copy to RAM
            if(tinf->got_entries > 0) {
                // Copy the GOT from flash to RAM
                // app_got_base is the base of the GOT in the RAM
                // this is where GOT will be copied to
                LOG_DBG("GOT in app stack: %p", app_got_base);

                // got_entries_base is the base of the GOT in the flash
                // tinf->got_entries number of entries from this location
                // needs to be copied into RAM
                uint32_t* got_entries_base = data_base + tinf->data_size;

                // While copying add the base address (app_data_base) in RAM to each element of the GOT
                // TODO: Add more explaination about this
                // Need to subtract the data_offset to get the location with respect to 0
                uint32_t data_offset = 0x10000000;
                for(uint8_t i = 0; i < tinf->got_entries; i++) {
                    if(*(got_entries_base+i) >= data_offset) {
                        // If the value is greater than data_offset then it is in the RAM section.
                        // So add the app_data_base to it.
                        *(app_got_base+i) = ((*(got_entries_base+i))-data_offset)+(uint32_t)((uintptr_t)app_data_base);
                    } else {
                        // else it is a relocation in the flash
                        // so add the flash->bin ie the base of the app in the flash to it
                        *(app_got_base+i) = (*(got_entries_base+i))+(uint32_t)((uintptr_t)tinf->bin);
                    }
                }
            }

            // If there is any bss in the app then set that much space to 0
            if(tinf->bss_size > 0) {
                // Set BSS section to 0
                memset(app_data_base+(tinf->data_size+tinf->got_entries), 0, tinf->bss_size*4);
                LOG_DBG("Data at bss section (RAM): 0x%08X", *(uint32_t*)(app_data_base+(tinf->data_size)));
            }
            //uint32_t* app_stack_got = app_data_base + tinf->data_size;
            //uint32_t* got_entries_base = data_base + tinf->data_size;
            //for(uint8_t i = 0; i < tinf->got_entries; i++) {
            //LOG_DBG("app_stack_got: 0x%08X: 0x%08X: 0x%08X and flash: 0x%08X", app_stack_got, *app_stack_got, *((uint32_t*)((uintptr_t)(*app_stack_got))), *got_entries_base++);
            //LOG_DBG("app_stack_got: 0x%08X: 0x%08X and flash: 0x%08X", app_stack_got, *app_stack_got, *got_entries_base++);
            //app_stack_got++;
            //}
        }
        
        // OR'ed with 1 to set the thumb bit, TODO: Check if this is handled internally by zephyr
        int (*app_main)() = (void*)(((uintptr_t)(tinf->bin))|1);
        LOG_DBG("App entry point: 0x%08X", app_main);
        // TODO: The following line gives segfault for some reason
        //LOG_DBG("Data at app entry point: 0x%08X", *((uint32_t*)(((uintptr_t)app_main)&0xFFFFFFFE)));

        //app_main(app_got_base);

        k_thread_create(&app_thread,
                        //(k_thread_stack_t*) app_stack_base,
                        app_stack,
                        app_stack_size,
                        (k_thread_entry_t)app_main,
                        //blinky,
                        app_got_base, NULL, NULL,
                        3, K_USER, K_NO_WAIT);

        return 0;
    } else {
        // If check fails then it probably is not an app or it is corrupted
        // Invalid app
        return APP_INVALID;
    }
}

