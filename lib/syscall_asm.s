.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.type jumptable, %object
.section .jump_table,"a",%progbits 
jumptable:
	.word set_led_2

.section .text.set_led
.globl set_led
.type  set_led,%function
set_led:
    push {r5}
    ldr r5, =jumptable
    ldr ip, [r5, #0]
    pop {r5}
    bx  ip
.size  set_led, .-set_led


