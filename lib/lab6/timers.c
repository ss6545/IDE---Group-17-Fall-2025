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
#include "lab6/leds.h"
#include "lab6/switches.h"
#include "lab6/timers.h"
#include "lab6/uart.h"

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
	TIMG0->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG0->CLKDIV = clk_div;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG0->COMMONREGS.CPS = prescaler;
	//disable counter
	TIMG0->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMG0->COUNTERREGS.LOAD = period;
	//set to load val which is the period, downcount, and repeat after event
	TIMG0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_ENABLED;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG0->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;

	//disable interrupts 
	__disable_irq();
	//clear zero event 
	TIMG0->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;
	//enable zero event
	TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;
	
	//register the interrupt w NVIC
	NVIC_EnableIRQ(TIMG0_INT_IRQn);
	//enable interrupts
	__enable_irq();
	
}

	//start (enable) the timer
	//TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;


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
	TIMG6->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMG6->CLKDIV = clk_div;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMG6->COMMONREGS.CPS = prescaler;
	//disable counter
	TIMG6->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMG6->COUNTERREGS.LOAD = period;
	//set to load val which is the period, downcount, and repeat after event
	TIMG6->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_ENABLED;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG6->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	
	//disable interrupts 
	__disable_irq();
	//clear zero event 
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;
	//enable zero event
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;
	
	//register the interrupt w NVIC
	NVIC_EnableIRQ(TIMG6_INT_IRQn);
	//enable interrupts
	__enable_irq();
	
	
}

	//start (enable) the timer
	//TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;


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
	TIMG12->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	
	
	//disable counter
	TIMG12->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMG12->COUNTERREGS.LOAD = period;
	//set to load val which is the period, downcount, and repeat after event
	TIMG12->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_ENABLED;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMG12->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	//disable interrupts 
	__disable_irq();
	//clear zero event 
	TIMG12->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;
	//enable zero event
	TIMG12->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;
	
	//register the interrupt w NVIC
	NVIC_EnableIRQ(TIMG12_INT_IRQn);
	//enable interrupts
	__enable_irq();


}

	//start (enable) the timer
	//TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;


/**
 * @brief Timer A0 module PWM initialization
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] period - Timer load value
 * @param[in] prescaler - Timer prescale value
 * @param[in] percenetDutyCycle - PWM duty cycle positive
 * @note Store period to be able to adjust duty cycle percentage later
*/
void TIMA0_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle, uint32_t clk_div) {

	//->check if TIMA0 module has power enabled
	if (!(TIMA0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		TIMA0->GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		TIMA0->GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;
	}//->we have enabled power	
	
	//->TIMERCLOCK (TIMCLK) Configuration
	//DOUBLE CHECKKKKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMA0->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMA0->CLKDIV = clk_div;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMA0->COMMONREGS.CPS = prescaler;
	//disable counter
	TIMA0->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMA0->COUNTERREGS.LOAD = period;
	//set to load val which is the period, downcount, and repeat after event
	TIMA0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_ENABLED;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMA0->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	switch (pin) {
	
		case 0:
			//->IOMUX pin stuff
			//Select PINCM25 for use as port I/O bc TIMA0 is PB8
			IOMUX->SECCFG.PINCM[24] |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM25_PF_TIMA0_CCP0);
			//->GPIO pin output stuff
			//->CLK via GPIO stuff for output over DIO8 bc PB8
			GPIOB->DOESET31_0 |= (GPIO_DOE31_0_DIO8_ENABLE);
			
			//->PWM Setup
			//Set the TIMx.CC_xy[0/1] value to configure the duty cycle 
			// 	-> index chooses channel and val at index determines capture or compare mode
			TIMA0->COUNTERREGS.CC_01[0] = (uint32_t)(period*(1-percentDutyCycle)) ;//TIMA0 channel 0 set to compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			//	-> index chooses channel and val at index determines capture or compare mode
			//TIMA0->COUNTERREGS.CCCTL_01[pin] = 0;//TIMA0 channel 0 needs control for compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			TIMA0->COUNTERREGS.CCCTL_01[0] = GPTIMER_CCCTL_01_COC_COMPARE;
			//configure CCP as output
			TIMA0->COMMONREGS.CCPD = GPTIMER_CCPD_C0CCP0_OUTPUT;//should be = or |= ???
			//In TIMx.CCACT_xy[0/1], set the CCP output action settings for compare events, zero events, load events, 
			//		software force action, or fault events (TIMA only).
			//	?? set this up for rising edge? like the compare ??
			TIMA0->COUNTERREGS.CCACT_01[0] = GPTIMER_CCACT_01_LACT_CCP_HIGH | GPTIMER_CCACT_01_CDACT_CCP_LOW;
			//In TIMx.OCTL_xy[0/1], set CCPO = 0 to select the signal generator output
			TIMA0->COUNTERREGS.OCTL_01[0] = GPTIMER_OCTL_01_CCPO_FUNCVAL;
			//Enable the corresponding CCP output by setting ODIS.C0CCPn to 1 for the corresponding counter n.
			TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW;
			//Configure polarity of the signal using the CCPOINV bit, 
			//	and configure CCPIV to specify the CCP output state while disabled.
			TIMA0->COUNTERREGS.OCTL_01[0] |= GPTIMER_OCTL_01_CCPOINV_NOINV | GPTIMER_OCTL_01_CCPIV_LOW;
		break;
		
		
		case 1:
			//->IOMUX pin stuff
			//Select PINCM29 for use as port I/O bc TIMA0 is PB12
			IOMUX->SECCFG.PINCM[28] |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM29_PF_TIMA0_CCP1);
			//->GPIO pin output stuff
			//->CLK via GPIO stuff for output over DIO12 bc PB12
			GPIOB->DOESET31_0 |= (GPIO_DOE31_0_DIO12_ENABLE);
			
			//->PWM Setup
			//Set the TIMx.CC_xy[0/1] value to configure the duty cycle 
			// 	-> index chooses channel and val at index determines capture or compare mode
			TIMA0->COUNTERREGS.CC_01[1] = (uint32_t)(period*percentDutyCycle) ;//TIMA0 channel 0 set to compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			//	-> index chooses channel and val at index determines capture or compare mode
			//TIMA0->COUNTERREGS.CCCTL_01[pin] = 0;//TIMA0 channel 0 needs control for compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			TIMA0->COUNTERREGS.CCCTL_01[1] = GPTIMER_CCCTL_01_COC_COMPARE;
			//configure CCP as output
			TIMA0->COMMONREGS.CCPD = GPTIMER_CCPD_C0CCP0_OUTPUT;//should be = or |= ???
			//In TIMx.CCACT_xy[0/1], set the CCP output action settings for compare events, zero events, load events, 
			//		software force action, or fault events (TIMA only).
			//	?? set this up for rising edge? like the compare ??
			TIMA0->COUNTERREGS.CCACT_01[1] = GPTIMER_CCACT_01_LACT_CCP_HIGH | GPTIMER_CCACT_01_CDACT_CCP_LOW;
			//In TIMx.OCTL_xy[0/1], set CCPO = 0 to select the signal generator output
			TIMA0->COUNTERREGS.OCTL_01[1] = GPTIMER_OCTL_01_CCPO_FUNCVAL;
			//Enable the corresponding CCP output by setting ODIS.C0CCPn to 1 for the corresponding counter n.
			TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW;
			//Configure polarity of the signal using the CCPOINV bit, 
			//	and configure CCPIV to specify the CCP output state while disabled.
			TIMA0->COUNTERREGS.OCTL_01[1] |= GPTIMER_OCTL_01_CCPOINV_NOINV | GPTIMER_OCTL_01_CCPIV_LOW;
		break;
		
		
		case 2:
			//->IOMUX pin stuff
			//Select PINCM43 for use as port I/O bc TIMA0 is PB17
			IOMUX->SECCFG.PINCM[28] |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM43_PF_TIMA0_CCP2);
			//->GPIO pin output stuff
			//->CLK via GPIO stuff for output over DIO17 bc PB17
			GPIOB->DOESET31_0 |= (GPIO_DOE31_0_DIO17_ENABLE);
			
			//->PWM Setup
			//Set the TIMx.CC_xy[0/1] value to configure the duty cycle 
			// 	-> index chooses channel and val at index determines capture or compare mode
			TIMA0->COUNTERREGS.CC_23[0] = (uint32_t)(period*percentDutyCycle) ;//TIMA0 channel 0 set to compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			//	-> index chooses channel and val at index determines capture or compare mode
			//TIMA0->COUNTERREGS.CCCTL_01[pin] = 0;//TIMA0 channel 0 needs control for compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			TIMA0->COUNTERREGS.CCCTL_23[0] = GPTIMER_CCCTL_23_COC_COMPARE;
			//configure CCP as output
			TIMA0->COMMONREGS.CCPD = GPTIMER_CCPD_C0CCP0_OUTPUT;//should be = or |= ???
			//In TIMx.CCACT_xy[0/1], set the CCP output action settings for compare events, zero events, load events, 
			//		software force action, or fault events (TIMA only).
			//	?? set this up for rising edge? like the compare ??
			TIMA0->COUNTERREGS.CCACT_23[0] = GPTIMER_CCACT_23_LACT_CCP_HIGH | GPTIMER_CCACT_23_CDACT_CCP_LOW;
			//In TIMx.OCTL_xy[0/1], set CCPO = 0 to select the signal generator output
			TIMA0->COUNTERREGS.OCTL_23[0] = GPTIMER_OCTL_23_CCPO_FUNCVAL;
			//Enable the corresponding CCP output by setting ODIS.C0CCPn to 1 for the corresponding counter n.
			TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW;
			//Configure polarity of the signal using the CCPOINV bit, 
			//	and configure CCPIV to specify the CCP output state while disabled.
			TIMA0->COUNTERREGS.OCTL_23[0] |= GPTIMER_OCTL_23_CCPOINV_NOINV | GPTIMER_OCTL_23_CCPIV_LOW;
		break;
	
		
		case 3:
			//->IOMUX pin stuff
			//Select PINCM30 for use as port I/O bc TIMA0 is PB13
			IOMUX->SECCFG.PINCM[29] |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM30_PF_TIMA0_CCP3);
			//->GPIO pin output stuff
			//->CLK via GPIO stuff for output over DIO13 bc PB13
			GPIOB->DOESET31_0 |= (GPIO_DOE31_0_DIO13_ENABLE);
			
			//->PWM Setup
			//Set the TIMx.CC_xy[0/1] value to configure the duty cycle 
			// 	-> index chooses channel and val at index determines capture or compare mode
			TIMA0->COUNTERREGS.CC_23[1] = (uint32_t)(period*percentDutyCycle) ;//TIMA0 channel 0 set to compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			//	-> index chooses channel and val at index determines capture or compare mode
			//TIMA0->COUNTERREGS.CCCTL_01[pin] = 0;//TIMA0 channel 0 needs control for compare
			//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
			TIMA0->COUNTERREGS.CCCTL_23[1] = GPTIMER_CCCTL_23_COC_COMPARE;
			//configure CCP as output
			TIMA0->COMMONREGS.CCPD = GPTIMER_CCPD_C0CCP0_OUTPUT;//should be = or |= ???
			//In TIMx.CCACT_xy[0/1], set the CCP output action settings for compare events, zero events, load events, 
			//		software force action, or fault events (TIMA only).
			//	?? set this up for rising edge? like the compare ??
			TIMA0->COUNTERREGS.CCACT_23[1] = GPTIMER_CCACT_23_LACT_CCP_HIGH | GPTIMER_CCACT_23_CDACT_CCP_LOW;
			//In TIMx.OCTL_xy[0/1], set CCPO = 0 to select the signal generator output
			TIMA0->COUNTERREGS.OCTL_23[1] = GPTIMER_OCTL_23_CCPO_FUNCVAL;
			//Enable the corresponding CCP output by setting ODIS.C0CCPn to 1 for the corresponding counter n.
			TIMA0->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW;
			//Configure polarity of the signal using the CCPOINV bit, 
			//	and configure CCPIV to specify the CCP output state while disabled.
			TIMA0->COUNTERREGS.OCTL_23[1] |= GPTIMER_OCTL_23_CCPOINV_NOINV | GPTIMER_OCTL_23_CCPIV_LOW;
		break;
	
		default:
			UART0_put((uint8_t*)"SHAWTY FIRE BURNING ON THE DANCEFLOOR OOWOAHoahOAH");
		break;
			
	}
	
	
	//Enable the counter by setting TIMx.CTRCTL.EN = 1.
	//	??again, = or |= ??
	TIMA0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_EN_MASK;
	
	
	//clk to 0
	//GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
	//TIMG0 w 100kHz freq
	TIMG0_init(160,1,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
	
}


/**
 * @brief Timer A1 module PWM initialization
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] period - Timer load value
 * @param[in] prescaler - Timer prescale value
 * @param[in] percenetDutyCycle - PWM duty cycle positive
 * @note Store period to be able to adjust duty cycle percentage later
*/
void TIMA1_PWM_init(uint8_t pin, uint32_t period, uint32_t prescaler, double percentDutyCycle, uint32_t clk_div) {

//->check if TIMA0 module has power enabled
	if (!(TIMA1->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset board 
		//	set the key first to unlock write capability, assert the new key, and set the clear bit
		TIMA1->GPRCM.RSTCTL |= GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		TIMA1->GPRCM.PWREN |= GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE;
	}//->we have enabled power
	
	//->TIMERCLOCK (TIMCLK) Configuration
	//DOUBLE CHECKKKKKKKKKKKKKKKKKKKKKKK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//select the TIMCLK clock source (BUSCLK, MFCLK, or LFCLK) using the CLKSEL register	
	TIMA1->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//Optionally divide the TIMCLK using CLKDIV.RATIO
	TIMA1->CLKDIV = clk_div;
	//In TIMx instances with prescalers, optionally set a prescaler using CPS.PCNT
	TIMA1->COMMONREGS.CPS = prescaler;
	//disable counter
	TIMA1->COUNTERREGS.CTRCTL = 0;
	//->counter value after enable
	//set load to period val
	TIMA1->COUNTERREGS.LOAD = period;
	//set to load val which is the period, downcount, and repeat after event
	TIMA1->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CVAE_LDVAL | GPTIMER_CTRCTL_CM_DOWN | GPTIMER_CTRCTL_REPEAT_REPEAT_1 | GPTIMER_CTRCTL_EN_ENABLED;
	//Enable the TIMCLK by setting CCLKCTL.CLKEN = 1
	TIMA1->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	
	//->IOMUX pin stuff
	//Select PINCM17 for use as port I/O bc TIMA1 is PB4
	IOMUX->SECCFG.PINCM[16] |= (IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM17_PF_TIMA1_CCP0);
	//->GPIO pin output stuff
	//->CLK via GPIO stuff for output over DIO8 bc PB4
	GPIOB->DOESET31_0 |= (GPIO_DOE31_0_DIO4_ENABLE);
	
	//->PWM Setup
	//Set the TIMx.CC_xy[0/1] value to configure the duty cycle 
	// 	-> index chooses channel and val at index determines capture or compare mode
	TIMA1->COUNTERREGS.CC_01[pin] = (uint32_t)(period*percentDutyCycle) ;//TIMA0 channel 0 set to compare
	//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
	//	-> index chooses channel and val at index determines capture or compare mode
	//TIMA0->COUNTERREGS.CCCTL_01[pin] = 0;//TIMA0 channel 0 needs control for compare
	//Set TIMx.CCCTL_xy[0/1].COC = 1 for compare mode
	TIMA1->COUNTERREGS.CCCTL_01[pin] = GPTIMER_CCCTL_01_COC_COMPARE;
	//configure CCP as output
	TIMA1->COMMONREGS.CCPD = GPTIMER_CCPD_C0CCP0_OUTPUT;//should be = or |= ???
	//In TIMx.CCACT_xy[0/1], set the CCP output action settings for compare events, zero events, load events, 
	//		software force action, or fault events (TIMA only).
	//	?? set this up for rising edge? like the compare ??
	TIMA1->COUNTERREGS.CCACT_01[pin] = GPTIMER_CCACT_01_LACT_CCP_HIGH | GPTIMER_CCACT_01_CDACT_CCP_LOW;
	//In TIMx.OCTL_xy[0/1], set CCPO = 0 to select the signal generator output
	TIMA1->COUNTERREGS.OCTL_01[pin] = GPTIMER_OCTL_01_CCPO_FUNCVAL;
	//Enable the corresponding CCP output by setting ODIS.C0CCPn to 1 for the corresponding counter n.
	TIMA1->COMMONREGS.ODIS = GPTIMER_ODIS_C0CCP0_CCP_OUTPUT_LOW;
	//Configure polarity of the signal using the CCPOINV bit, 
	//	and configure CCPIV to specify the CCP output state while disabled.
	TIMA1->COUNTERREGS.OCTL_01[pin] |= GPTIMER_OCTL_01_CCPOINV_NOINV | GPTIMER_OCTL_01_CCPIV_LOW;
	
	
	//Enable the counter by setting TIMx.CTRCTL.EN = 1.
	//	??again, = or |= ??
	TIMA1->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_EN_MASK;
	//clk to 0
	//GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;
	//TIMG0 w 100kHz freq
	TIMG0_init(160,1,GPTIMER_CLKDIV_RATIO_DIV_BY_1);
}


/**
 * @brief Change PWM duty cycle for all Timer A0 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
*/
void TIMA0_PWM_DutyCycle(uint8_t pin, uint32_t period, double percentDutyCycle) {

	// 	-> index chooses channel and val at index determines capture or compare mode
	TIMA0->COUNTERREGS.CC_01[pin] = (uint32_t)(period*percentDutyCycle) ;//TIMA0 channel 0 set to compare


}


/**
 * @brief Change PWM duty cycle for all Timer A1 channels
 * @param[in] pin - Timer PWM output pin / channel
 * @param[in] percentDutyCycle - Duty cycle to change to
*/
void TIMA1_PWM_DutyCycle(uint8_t pin, uint32_t period, double percentDutyCycle) {

	// 	-> index chooses channel and val at index determines capture or compare mode
	TIMA1->COUNTERREGS.CC_01[pin] = (uint32_t)(period*percentDutyCycle) ;//TIMA1 channel 0 set to compare

}


