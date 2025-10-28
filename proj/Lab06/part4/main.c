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
	while (timer < 40000){		// produces a delay of 10ms
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
	
	//servo motor center positioning
//	TIMA1_PWM_init(3200,199,7.5);
//	delay();
	UART0_put("set to center\r\n");
	while(1) {
	
	//->start of dc motor stuff
	for(int i=0;i<=100; i++){
		// need to increase the duty cycle for TIMA0 channel 0 from 0 to 100%
		TIMA0_PWM_init(0,3200,0,i);
		// channel 1 needs to be set to 0% for the whole time
		TIMA0_PWM_init(1,3200,0,0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	
	for(int i=100;i>=0; i--){
		// need to decrease the duty cycle for TIMA0 channel 0 from 100 to 0%
		TIMA0_PWM_init(0,3200,0,i);
		// channel 1 needs to be set to 0% for the whole time
		TIMA0_PWM_init(1,3200,0,0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	
	for(int i=0;i<=100; i++){
		// need to increase the duty cycle for TIMA0 channel 0 from 0 to 100%
		TIMA0_PWM_init(0,3200,0,0);
		// channel 1 needs to be set to 0% for the whole time
		TIMA0_PWM_init(1,3200,0,i);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	
	for(int i=100;i>=0; i--){
		// need to decrease the duty cycle for TIMA0 channel 0 from 100 to 0%
		TIMA0_PWM_init(0,3200,0,0);
		// channel 1 needs to be set to 0% for the whole time
		TIMA0_PWM_init(1,3200,0,i);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	//->end of dc motor stuff
		
	//->start of servo motor stuff
	//center to right
	TIMA1_PWM_init(3200,199,7.5);
		UART0_put("set to center\r\n");
	//center to right
	for(int i=76 ;i<=125 ; i++){
		// need to increase the duty cycle for TIMA0 channel 0 from 0 to 100%
		TIMA1_PWM_init(3200,199,(double)i/10.0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	UART0_put("went center to right\r\n");
	//right to left
	for(int i=124 ;i>=25 ; i--){
		// need to decrease the duty cycle for TIMA0 channel 0 from 100 to 0%
		TIMA1_PWM_init(3200,199,(double)i/10.0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	UART0_put("went right to left\r\n");
	//left to center
	for(int i=26;i<=75; i++){
		// need to increase the duty cycle for TIMA0 channel 0 from 0 to 100%
		TIMA1_PWM_init(3200,199,(double)i/10.0);
		UART0_printDec(i);
		UART0_put("\r\n");
		delay();	// wait for 10ms after each increase in duty cycle
	}
	UART0_put("came back to center\r\n");
	//break statement for testing purposes - REMOVE ONCE APPROVED
	//->end of servo motor stuff

	}
	
	return 0;

}
