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
#include "lab5/switches.h"
#include "lab5/uart.h"
#include "uart_extras.h"

void TIMG0_IRQHandler(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);

// state = 2 means initial state
static short toggle_sw1 = 2;
static bool toggle_LED1 = 0;
static short toggle_sw2 = 2;
static int color_counter = 0;
static long int ms_c = 0;		// millisecond counter for LED2
static short display_ms = 2;	// to ensure ms is displayed only once after clicking sw2

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

//main makes the LEDs (outputs) light up according to the inputs of?the?switches/
int main() {
	//->set up LED1 and S1
	LED1_init();
	S1_init_interrupt();
	LED2_init();
	S2_init_interrupt();
	UART0_init();
	//TIMG0 w 0.5Hz freq
	TIMG0_init(40000,199,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
	//TIMG6 w 0.5Hz freq
	//TIMG6_init(39999,199,GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	//TIMG6 w 2 Hz freq
	TIMG6_init(20000,99,GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	//TIMG12 w 1kHz freq
	TIMG12_init(32000);
	
	//int numS2presses = 0;
	
	
	while (1) {}//all other functionality will b done in the interrupts
		
			

	
return 0;

}



//Timer Interrupts
void TIMG0_IRQHandler(void) {
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; // clear the interrupt that caused the calling of this function (the zero event interrupt)
}

void TIMG6_IRQHandler(){
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; // clear the interrupt that caused the calling of this function (the zero event interrupt)
	if (toggle_sw1 == 1) {			//switch is on
		if (toggle_LED1 == 1) {		// if LED1 is on
			toggle_LED1 = 0;				// turn it off
			LED1_set(0);
		}
		else {										// if LED1 is off
			toggle_LED1 = 1;				// turn it on
			LED1_set(1);
		}
	}
	if (toggle_sw1 == 0) {											// switch is off
		toggle_LED1 = 0;					// turn LED off
		LED1_set(0);
//		// test
//		UART0_put("Disable TIMG6 after this\r\n\0");
		TIMG6->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;	// stop TIMG6 timer
	}
	
}

void TIMG12_IRQHandler(void) {
	if(toggle_sw2 == 1){				// if switch2 was pressed to turn it on
		if(!S2_pressed()) {				// wait till sw2 is released to work with the millisecond counting
			LED2_set(0);							// turn off previous color
			color_counter++;					// new LED color
			if (color_counter == 8){	// wrap around for invalid case
				color_counter = 1;
			}
			LED2_set(color_counter);	// turn on new color on LED2
			ms_c++;										// increase ms counter		
		}
	}
	if(toggle_sw2 == 0 && display_ms == 1) {
		if(!S2_pressed()) {				// wait till sw2 is released to work with the millisecond counting
			LED2_set(0);							// turn off previous color
			color_counter = 0;				// color = off
			UART0_put("Time elapsed: \0");
			UART0_printDec(ms_c);
			UART0_put(" ms\r\n\0");
			ms_c = 0;
			display_ms = 0;						// we are done displaying and do not need to display again until next cycle
		}
		else {										// if the switch ahs not been released yet, continue with sw2 on processes
			LED2_set(0);							// turn off previous color
			color_counter++;					// new LED color
			if (color_counter == 8){	// wrap around for invalid case
				color_counter = 1;
			}
			LED2_set(color_counter);	// turn on new color on LED2
			ms_c++;										// increase ms counter		
		}
	}
}

//Switch interrupts - GPIO0 (IIDX: 0) and GPIO1 (IIDX: 1)
void GROUP1_IRQHandler() {
	if ((GPIOA->CPU_INT.IIDX & GPIO_CPU_INT_IIDX_STAT_DIO18) == GPIO_CPU_INT_IIDX_STAT_DIO18){ // checking if sw1 is the reason for the interrupt
		//clear interrupt at pin for SW1	
		GPIOA->CPU_INT.ICLR |= GPIO_CPU_INT_ICLR_DIO18_CLR;
		// toggle
		if(toggle_sw1 == 1) {		// if on
//			// test
//			UART0_put("Turning switch off\r\n\0");
			toggle_sw1 = 0;		// turn switch off
		}
		else if(toggle_sw1 == 0 || toggle_sw1 == 2) { //if off
			TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;	// start (enable) TIMG6 timer
//			// test
//			UART0_put("Turning switch on\r\n\0");
			toggle_sw1 = 1;		// turn switch on
		}
		
	} 
	if ((GPIOB->CPU_INT.IIDX & GPIO_CPU_INT_IIDX_STAT_DIO21) == GPIO_CPU_INT_IIDX_STAT_DIO21) {
		//clear interrupt at pin for SW2	
		GPIOB->CPU_INT.ICLR |= GPIO_CPU_INT_ICLR_DIO21_CLR;
		if (toggle_sw2 == 0 || toggle_sw2 == 2) {
			toggle_sw2 = 1; // turn on sw2
		}
		else if (toggle_sw2 == 1) {
			toggle_sw2 = 0;	// turn off sw2
			display_ms = 1;
		}
	}
}
