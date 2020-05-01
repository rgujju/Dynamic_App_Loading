
#include "syscall.h"

#ifdef KERNEL

sys_struct sys = {
    .set_led = set_led,
};

#else
// The section ".sys_struct" will be at the beginning of the tinf which will be replaced
// The value DUMMY_SYS_ADDR is a dummy value which will be sanity checked before
// being replaced with actual sys_struct address on the mcu during app loading
const sys_struct *sys __attribute__((unused,section(".sys_struct"))) = (sys_struct*)(DUMMY_SYS_ADDR);

#endif

