
#include "FreeRTOS.h"
#include "task.h"
#include "string.h" //TODO: Replace with a minimal libc implementation
#include "stdlib.h" //TODO: Replace with a minimal libc implementation
#include "app_loader.h"

extern uint32_t sys;
// Tests:
// 2) Check name length =0,1,15,16,and greater, Check if it is 0s after name length
// 3) Check little endian/ big endian or whatever
// 4) malloc fails
// 5) app loading fails in xtaskCreateStatic
// 6) tinf->bin should fail

/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;
TaskHandle_t xHandle = NULL;

int8_t LoadApp(const uint8_t* tinf_img) {
    tinf_t* tinf = (tinf_t*)tinf_img;
    uint32_t dummy_sys_addr = DUMMY_SYS_ADDR;
    uint32_t* got_base;
    // Check if it is actually an APP
    // Check the magic, ie start of the header contains 'TINF'
    // and start of the binary part after header contains DUMMY_SYS_ADDR
    // DUMMY_SYS_ADDR is placed at the start using the application linker script
    uint32_t* data_base = tinf->bin+tinf->text_size;
    if(memcmp(tinf->magic, "TINF", 4) == 0 && memcmp(data_base, &dummy_sys_addr, 4) == 0) {
        // Valid TINF Format app
        //DBUG("App name: %s", tinf->app_name);
        //DBUG("App major version: %hu", tinf->major_version);
        //DBUG("App minor version: %hu", tinf->minor_version);
        //DBUG("App text size: %hu 32 bit word", tinf->text_size);
        //DBUG("App data size: %hu 32 bit word", tinf->data_size);
        //DBUG("App bss size: %hu 32 bit word", tinf->bss_size);
        //DBUG("App GOT entries: %ld", tinf->got_entries);

        // Allocate memory for data and bss section of the app on the heap
        uint32_t app_stack_size = tinf->data_size+tinf->got_entries+tinf->bss_size;
        //DBUG("Allocating app memory of %ld bytes",app_stack_size*4);
        StackType_t* app_stack = malloc((app_stack_size+10)*4);
        if(app_stack == NULL) {
            return APP_OOM;
        }
        uint32_t* app_stack_base = app_stack+app_stack_size;
        //DBUG("Application stack: 0x%08X", app_stack);
        if(app_stack_size != 0) {
            if(tinf->data_size > 0) {
                // Copy data from flash to the RAM we allocated above
                memcpy(app_stack, data_base, (tinf->data_size*4));
                //DBUG("Data at data section (flash): 0x%08X", *(uint32_t*)(tinf->bin+(tinf->text_size)));
                //DBUG("Data at data section (RAM): 0x%08X", *(uint32_t*)(app_stack));
                
                // Replace the sys_struct address to the one on the mcu
                // The start of the app_stack will point to start of the data section
                // This is where the sys_struct address was kept by the linker script
                *app_stack = (uintptr_t)&sys;
                DBUG("App sys_struct: 0x%08X", *app_stack);
            }
            if(tinf->got_entries > 0) {
                // app_stack_got is the base of the GOT in the RAM
                // this is where GOT will be copied to
                // app_stack_got will be used to increment the pointer
                uint32_t app_stack_got_base = (uintptr_t)(app_stack + tinf->data_size);
                uint32_t* app_stack_got = app_stack + tinf->data_size;
                DBUG("GOT in app stack: 0x%08X", app_stack_got_base);
                // got_entries_base is the base of the GOT in the flash
                // tinf->got_entries number of entries from this location
                // needs to be copied into RAM
                uint32_t* got_entries_base = data_base + tinf->data_size;
                // got_base is the value which will get loaded to r9
                // This is address of the base of GOT which will actually be used
                // by the app
                got_base = app_stack + tinf->data_size;
                // While copying add the base address of app_stack to each element of the GOT
                // TODO: Add more explaination about thi
                uint32_t data_offset = tinf->text_size*4;
                //DBUG("Data offset: 0x%08X",data_offset);
                for(uint8_t i = 0; i < tinf->got_entries; i++) {
                    *app_stack_got++ = ((*got_entries_base++)-data_offset)+(uint32_t)((uintptr_t)app_stack);
                }
            }
            if(tinf->bss_size > 0) {
                // Set BSS section to 0
                memset(app_stack+((tinf->text_size+tinf->data_size)*4), 0, tinf->bss_size*4);
                //DBUG("Data at bss section (RAM): 0x%08X", *(uint32_t*)(app_stack+(tinf->data_size)));
            }
            //uint32_t* app_stack_got = app_stack + tinf->data_size;
            //uint32_t* got_entries_base = data_base + tinf->data_size;
            //for(uint8_t i = 0; i < tinf->got_entries; i++) {
                //DBUG("app_stack_got: 0x%08X: 0x%08X: 0x%08X and flash: 0x%08X", app_stack_got, *app_stack_got, *((uint32_t*)((uintptr_t)(*app_stack_got))), *got_entries_base++);
                //DBUG("app_stack_got: 0x%08X: 0x%08X and flash: 0x%08X", app_stack_got, *app_stack_got, *got_entries_base++);
                //app_stack_got++;
            //}
            
        }
        TaskFunction_t app_main_orig = (TaskFunction_t)(tinf->bin);
        TaskFunction_t app_main = (TaskFunction_t)(((uintptr_t)(tinf->bin))|1); /* OR'ed with 1 to set the thumb bit */
        //DBUG("App entry point: 0x%08X", app_main);
        // TODO: The following line gives segfault for some reason
        //DBUG("Data at app entry point: 0x%08X", *((uint32_t*)(((uintptr_t)app_main)&0xFFFFFFFE)));
        //DBUG("Check entry point: 0x%08X Data: 0x%08lX", (uint32_t*)app_main_orig, *(uint32_t*)app_main_orig);

        //app_main(got_base);
        xHandle = xTaskCreateStatic(
                      app_main,       		/* Function that implements the task. */
                      (const char *)tinf->app_name,		/* Text name for the task. */
                      app_stack_size,		/* Number of indexes in the xStack array. */
                      got_base,    				/* Parameter passed into the task. */
                      tskIDLE_PRIORITY,		/* Priority at which the task is created. */
                      app_stack_base,          	/* Array to use as the task's stack. */
                      &xTaskBuffer);  		/* Variable to hold the task's data structure. */
        return 0;
    } else {
        // If check fails then it probably is not an app or it is corrupted
        // Invalid app
        return APP_INVALID;
    }
}

