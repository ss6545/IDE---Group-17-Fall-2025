/**
 * ******************************************************************************
 * @file    : switches.c
 * @brief   : Switches module c file
 * @details : Switches initialization and interaction
 * 
 * @author  : Akshay Narang
 * @date    : 09-2025
 * ******************************************************************************
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab5/leds.h"
#include "lab5/switches.h"

/**
 * @brief Switch 1 polling initialization
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void S1_init() {

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
		//Select PINCM1 for use as port I/O
		IOMUX->SECCFG.PINCM[39] |= (0x80 | 0x01);
		//Enable input
		IOMUX->SECCFG.PINCM[39] |= IOMUX_PINCM_INENA_ENABLE;
		//set up pull down resistor
		IOMUX->SECCFG.PINCM[39] |= IOMUX_PINCM_PIPD_ENABLE;
		
}

/**
 * @brief Switch 2 polling initialization
*/
void S2_init() {

	//->check if GPIOA module has power enabled
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOB->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_MASK;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		GPIOB->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
		//->we have enabled power, next 2 steps: pick the GPIOA module (IOMUX stuff) and enable it
		
		//->IOMUX stuff
		//Select PINCM1 for use as port I/O
		IOMUX->SECCFG.PINCM[48] |= (0x80 | 0x01);
		//Enable input
		IOMUX->SECCFG.PINCM[48] |= IOMUX_PINCM_INENA_ENABLE;
		//Invert the internal logic of the I/O
		IOMUX->SECCFG.PINCM[48] |= IOMUX_PINCM_INV_ENABLE;
		//set up pull up resistor
		IOMUX->SECCFG.PINCM[48] |= IOMUX_PINCM_PIPU_ENABLE;
		
}


/**
 * @brief Check if switch 1 was pressed
 * @return True(1)/False(0) if switch 1 was pressed
*/
int S1_pressed() {
	//check if input reg has a val of 1, meaning SW1 was pressed
	if (GPIOA->DIN31_0  & (0x01 << 18)) {
		return 1;
	}//if here, SW1 not pressed
	return 0;
}


/**
 * @brief Check if switch 2 was pressed
 * @return True(1)/False(0) if switch 2 was pressed
*/
int S2_pressed() {
	//check if input reg has a val of 1, meaning SW2 was pressed
	if (GPIOB->DIN31_0  & (0x01 << 21)) {
		return 1;
	}//if here, SW2 not pressed
	return 0;
}


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
	GPIOA->CPU_INT.ISET |= GPIO_CPU_INT_IMASK_DIO18_SET;
	//set polarity for interrupt (prelab) -> switch 1 triggers interrupt on rising edge
	GPIOA->POLARITY31_16 |= GPIO_POLARITY31_16_DIO18_RISE;
	//register interrupt w the NVIC - use NVIC_EnableIRQ([IRQn]) func and IRQn is an enumin mspm0g350x.h
	NVIC_EnableIRQ(GPIOA_INT_IRQn);
	//turn interrupts back on after modifications
	__enable_irq();
}


/**
 * @brief Switch 2 interrupt initialization
 * @note Use NVIC_EnableIRQ() to register IRQn with the NVIC
 *       Check out `cmsis_armclang.h`
*/
void S2_init_interrupt(void) {
	//turn off interrupts before making interrupt modifications
	__disable_irq();
	//initialize S2
	S2_init();
	//clear interrupt at the correct pin	
	GPIOB->CPU_INT.ICLR |= GPIO_CPU_INT_ICLR_DIO21_CLR;
	//enable the intterupt for the correct pin
	GPIOA->CPU_INT.ISET |= GPIO_CPU_INT_IMASK_DIO21_SET;
	//set polarity for interrupt (prelab) -> switch 1 triggers interrupt on rising edge
	GPIOA->POLARITY31_16 |= GPIO_POLARITY31_16_DIO21_FALL;
	//register interrupt w the NVIC - use NVIC_EnableIRQ([IRQn]) func and IRQn is an enumin mspm0g350x.h
	NVIC_EnableIRQ(GPIOB_INT_IRQn);
	//turn interrupts back on after modifications
	__enable_irq();
}

