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

void delay(void);
/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void delay() {
	volatile long timer = 0;
	while (timer < 40000){
		timer+=1;
	}
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
	while(1) {
	for(int i=0;i<=100; i++){
		// need to increase the duty cycle for TIMA0 channel 0 from 0 to 100%
		TIMA0_PWM_init(0,3200,0,i);
		// channel 1 needs to be set to 0% for the whole time
		TIMA1_PWM_init(3200,0,0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	
	for(int i=100;i>=0; i--){
		// need to decrease the duty cycle for TIMA0 channel 0 from 100 to 0%
		TIMA0_PWM_init(0,3200,0,i);
		// channel 1 needs to be set to 0% for the whole time
		TIMA1_PWM_init(3200,0,0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	
	for(int i=0;i<=100; i++){
		// need to increase the duty cycle for TIMA0 channel 0 from 0 to 100%
		TIMA0_PWM_init(0,3200,0,0);
		// channel 1 needs to be set to 0% for the whole time
		TIMA1_PWM_init(3200,0,i);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	
	for(int i=100;i>=0; i--){
		// need to decrease the duty cycle for TIMA0 channel 0 from 100 to 0%
		TIMA0_PWM_init(0,3200,0,0);
		// channel 1 needs to be set to 0% for the whole time
		TIMA1_PWM_init(3200,0,i);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
}
	
	
//	//20% duty cycle -------V & 10k Hz Freq calc-> TIMA0 is 32 bit so no prescaler
//	TIMA0_PWM_init(0,3200,0,20);	

//	//20% duty cycle -------V & 10k Hz Freq calc-> TIMA0 is 32 bit so no prescaler
//	TIMA0_PWM_init(1,3200,0,0);


	return 0;

}


