/******************************************************************************
Akshay Narang 
IDE Lab05 - 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab6/leds.h"
#include "lab6/switches.h"
#include "lab6/uart.h"
#include "lab6/timers.h"
#include "lab6/adc12.h"
#include "lab6/camera.h"
#include "uart_extras.h"

int main() {
	
	//->set up LED1 and S1
	LED1_init();
	S1_init_interrupt();
	LED2_init();
	S2_init_interrupt();
	UART0_init();
	ADC0_init();
	Camera_init();
	
	//20% duty cycle -------V & 10k Hz Freq calc-> TIMA0 is 32 bit so no prescaler
	TIMA0_PWM_init(0,3200,0,20,GPTIMER_CLKDIV_RATIO_DIV_BY_1);	




	return 0;

}


