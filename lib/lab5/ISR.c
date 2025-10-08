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

/**
 * @brief Switch 1 interrupt initialization
 * @note Use NVIC_EnableIRQ() to register IRQn with the NVIC
 *       Check out `cmsis_armclang.h`
 * @hint Keep the polarity in mind
*/
void S1_init_interrupt(void) {
	//turn off interrupts before making interrupt modifications
	__disable_irq();
	//initialize S1
	S1_init();	
	//clear interrupt at the correct pin	
	GPIOA->CPU_INT.ICLR |= GPIO_CPU_INT_ICLR_DIO18_CLR;
	//enable the intterupt for the correct pin
	GPIOA->CPU_INT.IMASK |= GPIO_CPU_INT_IMASK_DIO18_SET;
	//set polarity for interrupt (prelab) -> switch 1 triggers interrupt on rising edge
	GPIOA->POLARITY31_16 |= GPIO_POLARITY31_16_DIO18_RISE;
	//register interrupt w the NVIC - use NVIC_EnableIRQ([IRQn]) func and IRQn is an enumin mspm0g350x.h
	NVIC_EnableIRQ(GPIOA_INT_IRQn);
	//turn interrupts back on after modifications
	__enable_irq();
}


//Timer Interrupts
void TIMG0_IRQHandler(void) {
	//turn off interrupts before making interrupt modifications
	__disable_irq();
	
	//turn interrupts back on after modifications
	__enable_irq();
}

void TIMG6_IRQHandler(void) {}

void TIMG12_IRQHandler(void) {}

//Switch interrupts - GPIO0 (IIDX: 0) and GPIO1 (IIDX: 1)
void GROUP1_IRQHandler(void) {}
