.syntax unified
.thumb

.global AppStart 
.section  .AppStart
.type  AppStart, %function
AppStart: 
    mov     r9, r0       /* set GOT table address to r9. The value will passed as argument to the function */
