
#ifdef KERNEL
#include "utilities.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "stm32f4xx_hal.h"
#endif

#include "syscall.h"
uint8_t set_led_2(uint8_t Led_Num, uint8_t Led_State);
typedef void (*FUNCTION)(void);

#ifdef KERNEL
// This is the symbol jump table.
// Address of this table is at a fixed location in the memory
// It is stored in a fixed location by using the linker script
//static const FUNCTION jump_table[] = {
//    [0] = (FUNCTION) set_led_2
//};

#endif

//#ifndef KERNEL
/*
static int jump2func(uint32_t x){
   FUNCTION function = jump_table[x];
   register int ret asm ("r0");
   asm volatile("add    sp, #16      ;" 
                "mov    ip,%1        ;" 
                "pop    {r0,r1,r2,r3};"
                "sub    sp,#16       ;"
                "bx     ip           ;"    
                : "=r"  (ret)
                : "r"   (function)
                );
   return ret;
} 
*/
//asm ("push  {r0, r1, r2, r3}\n" 
//                     "ldr   r0, #0x080808    \n" 
//                   )
//#endif

/*
uint8_t set_led(uint8_t Led_Num, uint8_t Led_State){

#ifdef KERNEL
    // The actual code of the function is here.
    // the macro is controlled from the Makefile targets
    //switch(Led_Num){
    //    case 1:
    //        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, Led_State);
    //        break;
    //    case 2:
    //        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, Led_State);
    //        break;
    //}
    asm volatile("push   {r0,r1,r2,r3};"
                 "mov    r0,#0;"
                 "bl     jump2func;"
                );
    //return jump2func(0);
    return 4;
#else
    // Code to jump to this function is here
        
    return 2;
#endif
}

*/

uint8_t set_led_2(uint8_t Led_Num, uint8_t Led_State){

#ifdef KERNEL
    // The actual code of the function is here.
    // the macro is controlled from the Makefile targets
    //switch(Led_Num){
    //    case 1:
    //        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, Led_State);
    //        break;
    //    case 2:
            HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, Led_State);
    //        break;
    //}
    
    return 7;
    
#else
    // Code to jump to this function is here
        
    return 2;
#endif
}

