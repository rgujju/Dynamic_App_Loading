#include <stdint.h>
#include "led.h"

typedef struct sys_struct{
    uint8_t (*set_led)(uint8_t Led_Num, uint8_t Led_State);
}sys_struct;

#ifdef KERNEL
    sys_struct sys;
#else
   const  sys_struct *sys;
#endif
