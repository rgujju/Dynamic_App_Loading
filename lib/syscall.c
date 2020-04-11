
#ifdef KERNEL
#include "utilities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f4xx_hal.h"
#endif

#include "syscall.h"

#ifdef KERNEL

sys_struct sys __attribute__((section(".jumptbl"))) = {
    .set_led = set_led,
};

uint8_t set_led(uint8_t Led_Num, uint8_t Led_State) {
    // The actual code of the function is here.
    // the macro is controlled from the Makefile targets
    switch(Led_Num) {
        case 1:
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, Led_State);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, Led_State);
            break;
    }
    return 7;
}
#else

const sys_struct *sys __attribute__((unused)) = (sys_struct*)(0x200009cc);

#endif

