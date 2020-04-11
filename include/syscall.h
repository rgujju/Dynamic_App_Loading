#include <stdint.h>

#define LED1 1
#define LED2 2
#define LED_ON 1

typedef struct sys_struct{
    uint8_t (*set_led)(uint8_t Led_Num, uint8_t Led_State);
}sys_struct;

#ifdef KERNEL
    uint8_t set_led(uint8_t Led_Num, uint8_t Led_State);
    sys_struct sys;
#else
   const  sys_struct *sys;
#endif
