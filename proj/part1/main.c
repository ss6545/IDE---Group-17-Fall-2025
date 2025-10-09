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

void TIMG0_IRQHandler(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);


static bool toggle = 0;

//Switch interrupts - GPIO0 (IIDX: 0) and GPIO1 (IIDX: 1)
void GROUP1_IRQHandler(void);

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
	//TIMG0 w 0.5Hz freq
	TIMG0_init(39999,199,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
	//TIMG6 w 0.5Hz freq
	//TIMG6_init(39999,199,GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	//TIMG6 w 2 Hz freq
	TIMG6_init(19999,99,GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	//TIMG12 w 1kHz freq
	TIMG12_init(19);
	
	//int numS2presses = 0;
	
	
	while (1) {}//all other functionality will b done in the interrupts
		
			

	
return 0;

}



//Timer Interrupts
void TIMG0_IRQHandler(void) {
	TIMG0->CPU_INT.ICLR &= ~GPTIMER_CPU_INT_ICLR_Z_MASK;
	UART0_put("\n\rTIMG0 check");
}

void TIMG6_IRQHandler(void) {

	UART0_put("\n\rTIMG6 check");
	
	if (toggle) {//if 1, LED is flashing
	
		
	
	}
	
	
	while (1) {
		//tracks status of switch
		//0 is off
		//1 is toggling LED on and off for 0.5 sec
		int numS1presses=0;
		
		if (S1_pressed()) {
			numS1presses += 1;
			if (numS1presses == 2) numS1presses = 0;//if past 2nd press -> set back to off
			
			switch (numS1presses) {
				case 0:
					LED1_set(0);//off
					break;
				case 1:
					LED1_set(1);
					LED1_set(0);
					break;
				default:
					break;
			}
	
		}

	}
	
}

void TIMG12_IRQHandler(void) {}

//Switch interrupts - GPIO0 (IIDX: 0) and GPIO1 (IIDX: 1)
void GROUP1_IRQHandler(void) {}

