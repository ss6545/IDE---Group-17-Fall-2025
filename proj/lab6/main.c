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


/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void delay(void) {

	volatile long timer = 0;
	while (timer < 40000) timer+=1;
	
}

int main() {
	
	//->set up LED1 and S1
	LED1_init();
	S1_init_interrupt();
	LED2_init();
	S2_init_interrupt();
	UART0_init();
	ADC0_init();
	Camera_init();
	
	//->1ST SIGNOFF W OSCOPE CAPTURE
	//20% duty cycle -------V & 10k Hz Freq calc-> TIMA0 is 32 bit so no prescaler
	//TIMA0_PWM_init(0,3200,0,20);	

	//->2ND OSCOPE CAPTURE
	
	for (;;) {
	//initialize
	TIMA0_PWM_init(0,3200,0,0);//PIN,PERIOD,PRESCALER,%DUTY CYCLE	
	TIMA1_PWM_init(3200,0,20);//PERIOD,PRESCALER,%DUTY CYCLE	
	//delay for 10ms
	delay();
	//change duty cycle
	TIMA0_PWM_DutyCycle(0,3200,100);
	
		
		
		
	}
	
	
	return 0;

}


