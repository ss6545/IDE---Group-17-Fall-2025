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
	
	// ALSO CLEAR THE COUNTER CONTROL REG
	//->TIMERCLOCK (TIMCLK) Configuration
	//DOUBLE CHECKKKKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG0->COMMONREGS.CPS |= prescaler;//	or should i use GPTIMER_CPSV_CPSVAL_MAXIMUM?
	//counter value after enable
	TIMG0->COUNTERREGS.CTR |= period;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLE;

	
	__disable_irq();
	TIMG0->CPU_INT.ICLR |= GPTIMER_GEN_EVENT1_RIS_Z_CLR | GPTIMER_GEN_EVENT1_RIS_Z_SET;
	//ENABLE THE SAME WAY AS TIMER IS ENABLED
	TIMG0->CPU_INT.ISET |= GPTIMER_CTRCTL_EN_ENABLED;
	NVIC_EnableIRQ(TIMG0_INT_VECn);
	__enable_irq();
	
}


/**
 * @brief Timer G6 module initialization. General purpose timer
*/
void TIMG6_init(uint32_t period, uint32_t prescaler) {

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
	TIMG6->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_8;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG6->COMMONREGS.CPS |= prescaler;//	or should i use GPTIMER_CPSV_CPSVAL_MAXIMUM?
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG6->COMMONREGS.CCLKCTL |= 0x01;
	
	//->Counting Mode Control
	
	//->DOWNCOUNT
	//LOAD=0
	TIMG6->COUNTERREGS.LOAD |= 0;
	//WHEN THE 2 CONDITIONS R MET, LOAD VAL IS LOADED INTO CTR
	while ( (TIMG6->COUNTERREGS.CTR == 0) && (TIMG6->COUNTERREGS.RC == 1) ) {
		//LOAD VAL INTO CTR
		TIMG6->COUNTERREGS.CTR |= TIMG6->COUNTERREGS.LOAD;
	}
	//->PERIODIC
	TIMG6->COUNTERREGS.RC |= GPTIMER_CTRCTL_EN_MASK;
	//counter value after enable
	TIMG6->COUNTERREGS.CTR |= period;
	
	__disable_irq();
	TIMG6->CPU_INT.ICLR |= GPTIMER_GEN_EVENT1_RIS_Z_CLR | GPTIMER_GEN_EVENT1_RIS_Z_SET;
	TIMG6->CPU_INT.ISET |= GPTIMER_CTRCTL_EN_ENABLED;
	NVIC_EnableIRQ(TIMG6_INT_VECn);
	__enable_irq();
	
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
	TIMG12->CLKSEL |= GPTIMER_CLKSEL_MFCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG12->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG12->COMMONREGS.CPS |= 0x01;//	or should i use GPTIMER_CPSV_CPSVAL_MAXIMUM?
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG12->COMMONREGS.CCLKCTL |= 0x01;
	
	//->Counting Mode Control
	
	//->DOWNCOUNT
	//LOAD=0
	TIMG12->COUNTERREGS.LOAD |= 0;
	//WHEN THE 2 CONDITIONS R MET, LOAD VAL IS LOADED INTO CTR
	while ( (TIMG12->COUNTERREGS.CTR == 0) && (TIMG12->COUNTERREGS.RC == 1) ) {
		//LOAD VAL INTO CTR
		TIMG12->COUNTERREGS.CTR |= TIMG12->COUNTERREGS.LOAD;
	}
	//->PERIODIC
	TIMG12->COUNTERREGS.RC |= GPTIMER_CTRCTL_EN_MASK;
	//counter value after enable
	TIMG12->COUNTERREGS.CTR |= period;
	
	__disable_irq();
	TIMG6->CPU_INT.ICLR |= GPTIMER_GEN_EVENT1_RIS_Z_CLR | GPTIMER_GEN_EVENT1_RIS_Z_SET;
	TIMG6->CPU_INT.ISET |= GPTIMER_CTRCTL_EN_ENABLED;
	NVIC_EnableIRQ(TIMG12_INT_VECn);
	__enable_irq();
	



}
