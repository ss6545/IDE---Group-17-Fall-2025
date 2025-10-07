/**
 * ******************************************************************************
 * @file    : timers.c
 * @brief   : timers module c file
 * @details : timers initialization and interaction
 * 
 * @author  : Akshay Narang
 * @date    : 10-2025
 * ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab5/leds.h"
#include "lab5/switches.h"
#include "lab5/timers.h"


/**
 * @brief Timer G0 module initialization. General purpose timer
 * @note Timer G0 is in Power Domain 0. Check page 3 of the Data Sheet
*/
void TIMG0_init(uint32_t period, uint32_t prescaler) {
	//->check if TIMG0 module has power enabled
	if (!(TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		TIMG0->GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		TIMG0->GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;
	}//->we have enabled power
	
	//->TIMERCLOCK (TIMCLK) Configuration
	//DOUBLE CHECKKKKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG0->COMMONREGS.CPS |= prescaler;//	or should i use GPTIMER_CPSV_CPSVAL_MAXIMUM?
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG0->COMMONREGS.CCLKCTL |= 0x01;
	
	//->Counting Mode Control
	
	//->DOWNCOUNT
	//LOAD=0
	TIMG0->COUNTERREGS.LOAD |= 0;
	//WHEN THE 2 CONDITIONS R MET, LOAD VAL IS LOADED INTO CTR
	while ( (TIMG0->COUNTERREGS.CTR == 0) && (TIMG0->COUNTERREGS.RC == 1) ) {
		//LOAD VAL INTO CTR
		TIMG0->COUNTERREGS.CTR |= TIMG0->COUNTERREGS.LOAD;
	}
	//->PERIODIC
	TIMG0->COUNTERREGS.RC |= 0x01;
	//counter value after enable
	
	
	
}


/**
 * @brief Timer G6 module initialization. General purpose timer
*/
void TIMG6_init(uint32_t period, uint32_t prescaler);


/**
 * @brief Timer G12 module initialization. General purpose timer
 * @note Timer G12 has no prescaler
*/
void TIMG12_init(uint32_t period);
