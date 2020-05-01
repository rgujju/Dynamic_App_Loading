
#include "syscall.h"

uint8_t led_num = 0x02;
uint8_t led_status;

int main() {
	led_num-=1;
	led_status=0x01;
	sys->set_led(led_num, led_status);
    return 0;
}
