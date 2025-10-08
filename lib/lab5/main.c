/******************************************************************************
Akshay Narang 
IDE Lab05 - 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab5/leds.h"
#include "lab5/switches.h"
#include "lab5/uart.h"
#include "lab5/timers.h"


/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void keepLEDon(void) {

	volatile long timer = 0;
	while (timer < 2000000) timer+=1;
	
}

/*main makes the LEDs (outputs) light up according to the inputs of?the?switches*/
int main() {
	//->set up LED1 and S1
	LED1_init();
	S1_init_interrupt();
	LED2_init();
	S2_init_interrupt();
	UART0_init();
	//TIMG6 w 0.5Hz freq
	TIMG6_init(39999,199);
	//TIMG12 w 1kHz freq
	TIMG12_init(19);
	
	//int numS2presses = 0;
	
	
	while (1) {}//all other functionality will b done in the interrupts
		
			

	
return 0;

}
