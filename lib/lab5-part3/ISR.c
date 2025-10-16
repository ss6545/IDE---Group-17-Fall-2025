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
*/
