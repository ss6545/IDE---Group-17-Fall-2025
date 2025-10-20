/**
 * ******************************************************************************
 * @file    : leds.c
 * @brief   : LEDs module c file
 * @details : LED initialization and interaction
 * 
 * @author  : Akshay Narang
 * @date    : 09-2025
 * ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab6/leds.h"
#include "lab6/switches.h"



/**
 * @brief Initialze LED1
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void LED1_init() {
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
		IOMUX->SECCFG.PINCM[0] |= (0x80 | 0x01);
		//Invert the internal logic of the I/O
		IOMUX->SECCFG.PINCM[0] |= IOMUX_PINCM_INV_ENABLE;
		
		//->GPIO stuff for output over DIO0
		GPIOA->DOE31_0 |= (GPIO_DOE31_0_DIO0_ENABLE << 0);
		
		
	

}


/**
 * @brief Initialize LED2
 * @note You must account for each LED color
*/
void LED2_init() {
	
	//check if GPIOB module has power enabled
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOB->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_MASK;
		//->now enable power
		//set the key first to unlock write capability and enable power
		GPIOB->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
		//->IOMUX stuff
		//Select PINCMx for each LED use as port I/O
		IOMUX->SECCFG.PINCM[56] |= (0x80 | 0x01);//red
		IOMUX->SECCFG.PINCM[57] |= (0x80 | 0x01);//green
		IOMUX->SECCFG.PINCM[49] |= (0x80 | 0x01);//blue

		//->GPIO stuff
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO26_ENABLE);//led2-r
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO27_ENABLE);//led2-g
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO22_ENABLE);//led2-b
	
	
}


/**
 * @brief Set LED1 output state
 * @note ON, OFF
*/
void LED1_set(int stateLED1) {

	switch (stateLED1) {
		case 1: //on
			GPIOA->DOUTSET31_0 |= (0x01 << 0);
			break;
		case 0: //off
			GPIOA->DOUTCLR31_0 |= (0x01 << 0);
			break;
		
		default:
			break;
}
	
	

}

/**
 * @brief Set LED2 output state
 * @note RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF
 * 			 1, 	2,		 3,	   4,    5,       6,      7,		 0
*/
void LED2_set(int stateLED2) {

	switch (stateLED2) {
			case 0: //off
				GPIOB->DOUTCLR31_0 |= (0x01 << 26);//r
				GPIOB->DOUTCLR31_0 |= (0x01 << 27);//g
				GPIOB->DOUTCLR31_0 |= (0x01 << 22);//b
				break;
			case 1: //RED
				GPIOB->DOUTSET31_0 |= (0x01 << 26);//r
				break;
			case 2: //GREEN
				GPIOB->DOUTSET31_0 |= (0x01 << 27);//g
				break;
			case 3: //BLUE
				GPIOB->DOUTSET31_0 |= (0x01 << 22);//b
				break;
			case 4: //CYAN
				GPIOB->DOUTSET31_0 |= (0x01 << 22);//b
				GPIOB->DOUTSET31_0 |= (0x01 << 27);//g
				break;
			case 5: //MAGENTA
				GPIOB->DOUTSET31_0 |= (0x01 << 26);//r
				GPIOB->DOUTSET31_0 |= (0x01 << 22);//b
				break;
			case 6: //YELLOW
				GPIOB->DOUTSET31_0 |= (0x01 << 26);//r
				GPIOB->DOUTSET31_0 |= (0x01 << 27);//g
				break;
			case 7: //WHITE
				GPIOB->DOUTSET31_0 |= (0x01 << 26);//r
				GPIOB->DOUTSET31_0 |= (0x01 << 27);//g
				GPIOB->DOUTSET31_0 |= (0x01 << 22);//b
				break;
			default:
			break;
		}


}
