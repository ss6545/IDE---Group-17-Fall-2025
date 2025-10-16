/**
 * ******************************************************************************
 * @file    : adc12.h
 * @brief   : ADC module header file
 * @details : ADC initialization and interaction
 * @note    : ADC does not require IOMUX interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include "ti/devices/msp/peripherals/hw_adc12.h"
#include "lab5/adc12.h"
#include "../lib/sysctl.h"

/**
 * @brief Initialize ADC0
*/
void ADC0_init(){
	if(!(ADC0->ULLMEM.GPRCM.PWREN & ADC12_PWREN_ENABLE_ENABLE)) { // check if the PWREN is enabled or not
		ADC0->ULLMEM.GPRCM.PWREN = ADC12_RSTCTL_RESETSTKYCLR_CLR | ADC12_RSTCTL_KEY_UNLOCK_W | ADC12_RSTCTL_RESETASSERT_ASSERT; // clear, unlock and assert reset for the RSTCTL register
		ADC0->ULLMEM.GPRCM.PWREN = ADC12_PWREN_KEY_UNLOCK_W | ADC12_PWREN_ENABLE_ENABLE; // unlock the PWREN and enable it
		// set pa27 pin for analog (lab 1)
	}
	ADC0->ULLMEM.GPRCM.CLKCFG |= ADC12_CLKCFG_SAMPCLK_ULPCLK;		// setting ADCCLK to ultra low power clock
	ADC0->ULLMEM.CLKFREQ |= ADC12_CLKFREQ_FRANGE_RANGE40TO48;		// setting the ADCCLK freq range to the highest possible range
	ADC0->ULLMEM.CTL0 = 0;																// clear the CTL0 register before working on it
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_PWRDN_MANUAL; 				// setting the power down to manual so that the ADC does not turn off after coversion
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_SCLKDIV_DIV_BY_8;			// sampling clk div = 8
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_CONSEQ_SINGLE;				// single channel single conversion mode is set
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_TRIGSRC_SOFTWARE;			// setting conversion mode to software triggered
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SAMPMODE_AUTO;				// sampling mode set to auto
	ADC0->ULLMEM.CTL2 |= ADC12_CTL2_STARTADD_ADDR_00;			// choosing the MEMCTL0 register using STARTADD
	ADC0->ULLMEM.MEMCTL[0] |= ADC12_MEMCTL_CHANSEL_CHAN_0;	// setting channel 0 to chansel
	ADC0->ULLMEM.MEMCTL[0] = ADC12_MEMCTL_STIME_SEL_SCOMP0; // setting the desired sample timer value to SCOMP0
	
}


/**
 * @brief Retrieve the value from the ADC0
 * @note The ADC channel in use is set during initialization
 *       The channel is not the same as where the module stores the value
 * @return ADC0 processed value
*/
uint32_t ADC0_getVal(){	
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_ENC_ON;								// turnign on encoding to enable ADC conversions
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SC_START;							// starting the SC mode so that time is determined by the sampling clock
	while(1){
		if(!(ADC0->ULLMEM.CTL0 & ADC12_CTL0_ENC_ON)){				// wait for ENC to turn off automatically after receiving data
			return ADC0->ULLMEM.MEMRES[0] & ADC12_ULLMEM_MEMRES_DATA_MASK;		// get the data from the MEMRES and return it
		}
	}
	return 0;
}

