#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include "adc12.h"
#include "timers.h"
#include "lab5/uart.h"


int main(){
	TIMG6_init(1599, 24999);
	UART0_init();
	ADC0_init();
	
	return 0;
}