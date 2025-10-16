/**
 * ******************************************************************************
 * @file    : camera.c for part 3 of lab 5
 * @brief   : Camera module header file
 * @details : Parallax TSL1401-DB Linescan Camera initialization and interaction
 * @note    : Reserves the use of Timers G0 (CLK) and G6 (SI)
 *            and ADC0 channel 0 
 *
 * @author 
 * @date 
 * ******************************************************************************
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab5/timers.h"
#include "lab5/adc12.h"
#include "lab5/camera.h"
#include "lab5/uart.h"

//**part3 stuff**
//1 done reading data, 0 if still reading data
static bool cameraData_complete = 0;
//array to hold camera info
static uint16_t cameraData[128];
//counter for cameraData array
static int pixelCounter = 0;

void TIMG0_IRQHandler(void);
void TIMG6_IRQHandler(void);
void TIMG12_IRQHandler(void);


/**
 * @brief Initialize camera associated components
*/
void Camera_init(void) {

	//->check if GPIOA module has power enabled
	if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOA->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		GPIOA->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
	//->we have enabled power, next 2 steps: pick the GPIOA module (IOMUX stuff) and enable it
		
	//->IOMUX stuff
	//Select PINCM3 for use as port I/O bc PINCM3
	IOMUX->SECCFG.PINCM[2] |= (0x80 | 0x01);
	//->SI via GPIO stuff for output over DIO27 bc PA28
	GPIOA->DOE31_0 |= (GPIO_DOE31_0_DIO28_ENABLE);
	
	//->IOMUX stuff
	//Select PINCM34 for use as port I/O bc PINCM34
	IOMUX->SECCFG.PINCM[33] |= (0x80 | 0x01);
	//->CLK via GPIO stuff for output over DIO11 bc PA12
	GPIOA->DOE31_0 |= (GPIO_DOE31_0_DIO12_ENABLE);
	//ADC Init
	ADC0_init();
	//TIMG0 w 100kHz freq
	TIMG0_init(2,159,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
	//TIMG6 val for 7.5 ms -> freq = 1/7.5
	TIMG6_init(8000,31,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
	
}


/**
 * @brief Checks whether camer data is ready to retrieve
 * @note Make sure to check all data is available
 * @return True(1)/False(0) if camera data is ready
*/
uint8_t Camera_isDataReady(void) {
	return (uint8_t)cameraData_complete;
}


/**
 * @brief Retrieves pointer to camera data array
 * @return Pointer to global data array stored locally in this file
*/
uint16_t* Camera_getData(void){
	return cameraData;
}




//Timer Interrupts
void TIMG0_IRQHandler(void) {
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;//clear the interrupt - it happened and we boutta do stuff abt it
	//UART0_put("\n\rTIMG0 check");
	//**part 3 stuff**
	//->pulse CLK pin
	//CLK HIGH
	GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO12_SET;
	//CLK LOW
	GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
	
	//->check if cameraData is full; 
	//if full: set the cameraData_complete flag, 
	//				 disable the CLK timer, 
	//				 reset the index.
	if (pixelCounter >= 128) {//if cameraData is full
		cameraData_complete = 1;
		TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
		pixelCounter = 0;
	}
	else {//->if here, need to fill up cameraData 
		//get value from ADC and store it at currnet index of cameraData
		cameraData[pixelCounter] = (uint16_t)ADC0_getVal();
		//increment index
		pixelCounter++;
	}
}

void TIMG6_IRQHandler(void) {
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;//clear the interrupt - it happened and we boutta do stuff abt it
	//UART0_put("\n\rTIMG6 check");
	//**part 3 stuff**
	//turn off timg0
	TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_MASK;
	//->if done reading data, then begin new capture
	if (cameraData_complete) {
		//1-SI,CLK LOW
		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO28_CLR;
		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
		//2-SI HIGH
		GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO28_SET;
		//3-CLK HIGH
		GPIOA->DOUTSET31_0 |= GPIO_DOUTSET31_0_DIO12_SET;
		//4-SI,CLK LOW
		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO28_CLR;
		GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
		
		
		pixelCounter = 0;
	}
	//->ENABLE TIMG0
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;
}
