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
void TIMG0_init(uint32_t period, uint32_t prescaler, uint32_t clk_div) {
	//->check if TIMG0 module has power enabled
	if (!(TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		TIMG0->GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		TIMG0->GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;
	}//->we have enabled power
	
	// ALSO CLEAR THE COUNTER CONTROL REG
	//->TIMERCLOCK (TIMCLK) Configuration
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG0->CLKDIV |= clk_div;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG0->COMMONREGS.CPS |= prescaler;
	//disable counter
	TIMG0->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMG0->COUNTERREGS.LOAD |= period;
	//set to load val which is the period, downcount, and repeat after event
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

	//disable interrupts 
	__disable_irq();
	//clear zero event 
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;
	//enable zero event
	TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;
	
	//register the interrupt w NVIC
	NVIC_EnableIRQ(TIMG0_INT_IRQn);
	//enable interrupts
	__enable_irq();
	
	//start (enable) the timer
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;
}


/**
 * @brief Timer G6 module initialization. General purpose timer
*/
void TIMG6_init(uint32_t period, uint32_t prescaler, uint32_t clk_div) {

	//->check if TIMG0 module has power enabled
	if (!(TIMG6->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		TIMG6->GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		TIMG6->GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;
	}//->we have enabled power
	
	//->TIMERCLOCK (TIMCLK) Configuration
	//DOUBLE CHECKKKKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMG6->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG6->CLKDIV |= clk_div;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG6->COMMONREGS.CPS |= prescaler;
	//disable counter
	TIMG6->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMG6->COUNTERREGS.LOAD |= period;
	//set to load val which is the period, downcount, and repeat after event
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG6->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	//disable interrupts 
	__disable_irq();
	//clear zero event 
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;
	//enable zero event
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;
	
	//register the interrupt w NVIC
	NVIC_EnableIRQ(TIMG6_INT_IRQn);
	//enable interrupts
	__enable_irq();
	
	//not starting timer here
	
}


/**
 * @brief Timer G12 module initialization. General purpose timer
 * @note Timer G12 has no prescaler
*/
void TIMG12_init(uint32_t period) {

	//->check if TIMG0 module has power enabled
	if (!(TIMG12->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		TIMG12->GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		TIMG12->GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;
	}//->we have enabled power
	
	//->TIMERCLOCK (TIMCLK) Configuration
	//DOUBLE CHECKKKKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMG12->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG12->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	
	
	//disable counter
	TIMG12->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMG12->COUNTERREGS.LOAD |= period;
	//set to load val which is the period, downcount, and repeat after event
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG12->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	//disable interrupts 
	__disable_irq();
	//clear zero event 
	TIMG12->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;
	//enable zero event
	TIMG12->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_MASK;
	
	//register the interrupt w NVIC
	NVIC_EnableIRQ(TIMG12_INT_IRQn);
	//enable interrupts
	__enable_irq();
	
	//start (enable) the timer
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;


}

