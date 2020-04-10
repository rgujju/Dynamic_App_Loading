.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.type jumptable, %object
.section .jump_table,"a",%progbits 
jumptable:
	.word set_led_2

.section .text.jump2func
.globl jump2func
.type jump2func,%function
jump2func:
    ldr r2, =jumptable
    ldr ip, [r2, r0]
    pop {r0,r1,r2,r3}
    bx  ip

.section .text.set_led
.globl set_led
.type  set_led,%function
set_led:
    push {r0,r1,r2,r3}
    mov  r0, #0
    b    jump2func
.size  set_led, .-set_led


