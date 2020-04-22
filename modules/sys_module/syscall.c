
// User modules
#include "syscall.h"
#include "led.h"

#ifdef KERNEL

sys_struct sys __attribute__((section(".jumptbl"))) = {
    .set_led = set_led,
};

#else

const sys_struct *sys __attribute__((unused)) = (sys_struct*)(0x200009cc);

#endif

