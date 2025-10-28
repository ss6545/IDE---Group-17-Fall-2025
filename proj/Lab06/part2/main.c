/******************************************************************************
Akshay Narang, Sahima Sachdev
IDE Lab06 - part 2 - main file
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
#include "lab6/stepper.h"

void delay(int i);
/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void delay(int i) {
	volatile long timer = 0;
	while (timer < i){
		timer+=1;
	}
}

int main(){
	UART0_init();
	ADC0_init();
	Coil_A_init();
	Coil_B_init();
	Coil_C_init();
	Coil_D_init();
	LED2_init();
	
	int forward = 1;
	int phase = 0;
	
	while (1){
//		LED2_set(1);
//		delay(40000);
//		LED2_set(0);
//		LED2_set(2);
//		delay(40000);
//		LED2_set(0);
//		LED2_set(3);
//		delay(40000);
//		LED2_set(0);
//		LED2_set(4);
//		delay(40000);
//		LED2_set(0);
//		LED2_set(5);
//		delay(40000);
//		LED2_set(0);
		
		
		
		
		// Turn off all coils , Set GPIO pins to 0
		GPIOB->DOUTCLR31_0 |= (GPIO_DOUTCLR31_0_DIO6_CLR);
		GPIOB->DOUTCLR31_0 |= (GPIO_DOUTCLR31_0_DIO7_CLR);
		GPIOB->DOUTCLR31_0 |= (GPIO_DOUTCLR31_0_DIO0_CLR);
		GPIOB->DOUTCLR31_0 |= (GPIO_DOUTCLR31_0_DIO16_CLR);
		// Set one pin high at a time
		if (forward) {
			if ( phase == 0) { 
				/* turn on coil A */
				GPIOB->DOUTSET31_0 |= (GPIO_DOUTSET31_0_DIO6_SET); 
//				LED2_set(0);
//				LED2_set(1);
				phase ++;
			} // A ,1 a
			else if ( phase == 1) { 
				/* turn on coil B */ 
				GPIOB->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO7_SET; 
//				LED2_set(0);
//				LED2_set(2);
				phase ++;
			} // B ,2 a
			else if ( phase == 2) { 
				/* turn on coil C */
				GPIOB->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO0_SET;
//				LED2_set(0);
//				LED2_set(3);
				phase ++;
			} // C ,1 b
			else { 
				/* turn on coil D */ 
				GPIOB->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO16_SET;
//				LED2_set(0);
//				LED2_set(4);
				phase =0;
			} // D ,2 b
		}
		else { // reverse
			if ( phase == 0) { 
				/* turn on coil D */
				GPIOB->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO16_SET;
				phase ++;
			} // D ,2 b
			else if ( phase == 1) { 
				/* turn on coil C */
				GPIOB->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO0_SET;
				phase ++;
			} // C ,1 b
			else if ( phase == 2) { 
				/* turn on coil B */
				GPIOB->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO7_SET; 
				phase ++;
			} // B ,2 a
			else { 
				/* turn on coil A */
				GPIOB->DOUTSET31_0 |= (GPIO_DOUTSET31_0_DIO6_SET); 
				phase =0;
			} // A ,1 a
}
// Note - you need to write your own delay function
delay (4000000); // smaller values = faster speed
}
	return 0;
}