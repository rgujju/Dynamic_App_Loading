
#include "sysled.h"

uint8_t led_num = 0x02;
uint8_t led_status;

uint8_t dummy_function_2(){
    return 0x01;
}

uint8_t dummy_function(){
    uint8_t led_status = dummy_function_2();
    return led_status;
}

// dummy_function, dummy_function_2 and the global variables
// are there just to verify if the relocation is actually working

int main() {
	led_num-=1;
	led_status=dummy_function();
	SetLed(led_num, led_status);
    return 0;
}
