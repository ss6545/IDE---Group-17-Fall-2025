/**
 * ******************************************************************************
 * @file    : stepper.c
 * @brief   : stepper module c file
 * @details : stepper coils initialization
 * 
 * @authors : Akshay Narang, Sahima Sachdev
 * @date    : 10-2025
 * ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab6/stepper.h"

void Coil_A_init(void) {		// PB6, PINCM23
	//->check if GPIOA module has power enabled
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOB->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		GPIOB->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
		//->we have enabled power, next 2 steps: pick the GPIOA module (IOMUX stuff) and enable it
		
		//->IOMUX stuff
		//Select PINCM1 for use as port I/O
		IOMUX->SECCFG.PINCM[IOMUX_PINCM23] |= (IOMUX_PINCM_PC_CONNECTED | 0x01);
//		//Invert the internal logic of the I/O
//		IOMUX->SECCFG.PINCM[IOMUX_PINCM23] |= IOMUX_PINCM_INV_ENABLE;
		
		//->GPIO stuff for output over DIO06
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO6_ENABLE << 0);
}

void Coil_B_init(void) {		// PB7, PINCM24
	//->check if GPIOA module has power enabled
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOB->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		GPIOB->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
		//->we have enabled power, next 2 steps: pick the GPIOA module (IOMUX stuff) and enable it
		
		//->IOMUX stuff
		//Select PINCM1 for use as port I/O
		IOMUX->SECCFG.PINCM[IOMUX_PINCM24] |= (IOMUX_PINCM_PC_CONNECTED | 0x01);
//		//Invert the internal logic of the I/O
//		IOMUX->SECCFG.PINCM[IOMUX_PINCM24] |= IOMUX_PINCM_INV_ENABLE;
		
		//->GPIO stuff for output over DIO07
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO7_ENABLE << 0);
}

void Coil_C_init() {		// PB0, PINCM12
	//->check if GPIOA module has power enabled
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOB->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		GPIOB->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
		//->we have enabled power, next 2 steps: pick the GPIOA module (IOMUX stuff) and enable it
		
		//->IOMUX stuff
		//Select PINCM1 for use as port I/O
		IOMUX->SECCFG.PINCM[IOMUX_PINCM12] |= (IOMUX_PINCM_PC_CONNECTED | 0x01);
//		//Invert the internal logic of the I/O
//		IOMUX->SECCFG.PINCM[IOMUX_PINCM23] |= IOMUX_PINCM_INV_ENABLE;
		
		//->GPIO stuff for output over DIO06
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO0_ENABLE << 0);
}

void Coil_D_init() {		// PB16, PINCM33
	//->check if GPIOA module has power enabled
	if (!(GPIOB->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		GPIOB->GPRCM.RSTCTL |= GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		GPIOB->GPRCM.PWREN |= GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE;
	}
		//->we have enabled power, next 2 steps: pick the GPIOA module (IOMUX stuff) and enable it
		
		//->IOMUX stuff
		//Select PINCM1 for use as port I/O
		IOMUX->SECCFG.PINCM[IOMUX_PINCM33] |= (IOMUX_PINCM_PC_CONNECTED | 0x01);
//		//Invert the internal logic of the I/O
//		IOMUX->SECCFG.PINCM[IOMUX_PINCM23] |= IOMUX_PINCM_INV_ENABLE;
		
		//->GPIO stuff for output over DIO06
		GPIOB->DOE31_0 |= (GPIO_DOE31_0_DIO16_ENABLE << 0);
	
}
