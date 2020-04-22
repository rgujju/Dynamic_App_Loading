
#ifdef KERNEL

#include "utilities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f4xx_hal.h"

#include "led.h"

uint8_t set_led(uint8_t Led_Num, uint8_t Led_State) {
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

#endif
