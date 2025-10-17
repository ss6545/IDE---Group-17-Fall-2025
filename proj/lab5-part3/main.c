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
#include "lab5/adc12.h"
#include "lab5/camera.h"
#include "uart_extras.h"



//**part1 stuff**
//static bool toggle = 0;
//bool tim12_running = 1;
//double ms_counter = 0;


//**part3 stuff**
//1 done reading data, 0 if still reading data
//static bool cameraData_complete = 0;
//array to hold camera info
//static uint16_t cameraData[128];
//counter for cameraData array
//static int pixelCounter = 0;

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
	ADC0_init();
	Camera_init();
	
	//TIMG0 w 0.5Hz freq
	//TIMG0_init(40000,199,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
	//TIMG6 w 0.5Hz freq
	//TIMG6_init(39999,199,GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	//TIMG6 w 2 Hz freq
	//TIMG6_init(20000,99,GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	//TIMG12 w 1kHz freq
	//TIMG12_init(32000);
	
	
	//TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;//enable the timer
	//TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;//enable the timer
	//TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;//enable the timer
	

	//string holder for cameraData output to putty 
	//char stringHolder[8];
	//holds cameraData pointer
	uint16_t* cameraData_ptr = Camera_getData();
	
	while (1) {
		
		if (Camera_isDataReady()) {
			//->output camera data
			UART0_put("-1\r\n");//start
			
			for (int i = 0; i < 128; i++) {
				//update stringHolder's first element w the camera data on each iteration
				//snprintf(stringHolder, sizeof(stringHolder),"%u\r\n",cameraData_ptr[i]);
				//UART0_put(stringHolder);//output the current version of stringHolder
				UART0_printDec(cameraData_ptr[i]);
				UART0_put("\r\n");
			}
		
			UART0_put("-2\r\n");//stop

		}
		
	
	}//all other functionality will b done in the interrupts
		
			

	
return 0;

}



//Timer Interrupts

//void TIMG0_IRQHandler(void) {
//	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;//clear the interrupt - it happened and we boutta do stuff abt it
//	
//	UART0_put("\n\rTIMG0 check");
//	
//	//**part 3 stuff**
////	//->pulse CLK pin
////	//CLK HIGH
////	GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO12_SET;
////	//CLK LOW
////	GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
////	
////	//->check if cameraData is full; 
////	//if full: set the cameraData_complete flag, 
////	//				 disable the CLK timer, 
////	//				 reset the index.
////	if (pixelCounter >= 128) {//if cameraData is full
////		cameraData_complete = 1;
////		TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
////		pixelCounter = 0;
////	}
////	else {//->if here, need to fill up cameraData 
////		//get value from ADC and store it at currnet index of cameraData
////		cameraData[pixelCounter] = (uint16_t)ADC0_getVal();
////		//increment index
////		pixelCounter++;
////	}
//}

//void TIMG6_IRQHandler(void) {
//	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;//clear the interrupt - it happened and we boutta do stuff abt it
//	UART0_put("\n\rTIMG6 check");
//	
//	//**part 3 stuff**
////	//turn off timg0
////	TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
////	//->if done reading data, then begin new capture
////	if (cameraData_complete) {
////		//1-SI,CLK LOW
////		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO28_CLR;
////		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
////		//2-SI HIGH
////		GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO28_SET;
////		//3-CLK HIGH
////		GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO12_SET;
////		//4-SI,CLK LOW
////		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO28_CLR;
////		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
////		
////		
////		pixelCounter = 0;
////	}
////	//->ENABLE TIMG0
////	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;
//	
//	
//	
//	
//	
//	//old stuff from lab 1
////	if (toggle) {//if 1, LED is flashing

////		
////	
////	}
////	
////	
////	while (1) {
////		//tracks status of switch
////		//0 is off
////		//1 is toggling LED on and off for 0.5 sec
////		int numS1presses=0;
////		
////		if (S1_pressed()) {
////			numS1presses += 1;
////			if (numS1presses == 2) numS1presses = 0;//if past 2nd press -> set back to off
////			
////			switch (numS1presses) {
////				case 0:
////					LED1_set(0);//off
////					break;
////				case 1:
////					LED1_set(1);
////					LED1_set(0);
////					break;
////				default:
////					break;
////			}
////	
////		}

////	}



//	
//}

//void TIMG12_IRQHandler(void) {

//	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;//clear the interrupt - it happened and we boutta do stuff abt it
//	
//	UART0_put("\n\rTIMG12 check");
//	if (tim12_running) {
//		ms_counter++;
//		UART0_put("\n\rTIMG12 ms_counter incremented check");
//	}
//}

////Switch interrupts - GPIO0 (IIDX: 0) and GPIO1 (IIDX: 1)
//void GROUP1_IRQHandler(void) {}

