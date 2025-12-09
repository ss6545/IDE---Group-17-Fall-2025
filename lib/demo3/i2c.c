/**
 * ******************************************************************************
 * @file    : i2c.h
 * @brief   : I2C module header file
 * @details : I2C initialization and interaction
 * @note    : TI documentation switches terminology around, code is behind
 *             Controller <-> Master
 *             Target     <-> Slave
 *             This changes register access from what the documentation specifies
 *
 * @author  : Akshay Narang
 * @date    : 9/19/25
 * ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include "demo3/i2c.h"

/**
 * @brief Initialize I2C1
 * @param[in] targetAddress - Passed by OLED file. I2C target/listener address
 * @note OLED file looks for this exact prototype
*/
void I2C1_init(uint16_t targetAddress){

	//->check if I2C1 module has power enabled
	if (!(I2C1->GPRCM.PWREN & I2C_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset  
		//	set the clear bit, set the key first to unlock write capability, set the assert key
		I2C1->GPRCM.RSTCTL |= I2C_RSTCTL_RESETSTKYCLR_CLR | I2C_RSTCTL_KEY_UNLOCK_W | I2C_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		I2C1->GPRCM.PWREN |= I2C_PWREN_KEY_UNLOCK_W | I2C_PWREN_ENABLE_ENABLE;
	}
	
	
	//IOMUX
	//->configure the IOMUX->SECCFG.PINCM for TX and RX
	//Select PINCM15 AND 16 for use as port I/O -> RX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM15_PF_I2C1_SCL;
	//Enable input for RX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15] |= IOMUX_PINCM_INENA_ENABLE;
	//Select PINCM10 for use as port I/O -> RX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM16_PF_I2C1_SDA;
	//Enable input for RX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16] |= IOMUX_PINCM_INENA_ENABLE;
	
	//select CLK to BUSCLK
	I2C1->CLKSEL |= I2C_CLKSEL_BUSCLK_SEL_ENABLE;
	//set CLK DIV ratio to 1
	I2C1->CLKDIV |= I2C_CLKDIV_RATIO_DIV_BY_1;
	//glitch filter control reg -> disable analog glitch suppression
	I2C1->GFCTL &= ~I2C_GFCTL_AGFEN_MASK;
	//clear controller control reg (code: MCTR, TRM: CCTR or MTPR?)
	I2C1->MASTER.MCTR = 0x00;
	//clock select and I2C speed
	I2C1->MASTER.MTPR |= 0x1F;//can leave hardcoded theoretically...-Trevor
	
	
	//->controller FIFO control reg to set RX FIFO to trigger when it got >= 1 byte and TX FIFO to trigger when TX FIFO is empty
	I2C1->MASTER.MFIFOCTL |= I2C_MFIFOCTL_RXTRIG_LEVEL_1 | I2C_MFIFOCTL_TXTRIG_EMPTY;

	
	//OLED cant do clock stretching so disable it w the Controller Configuration Register 
	I2C1->MASTER.MCR &= ~I2C_MCR_CLKSTRETCH_MASK;
	//set the target addr that was passed into the init function using the Controller Target Addr Reg
	I2C1->MASTER.MSA |= ((unsigned int)targetAddress<<I2C_MSA_SADDR_OFS);
	//enable I2C controller using the Controller Configuration Reg
	I2C1->MASTER.MCR |= I2C_MCR_ACTIVE_ENABLE;

}


/**
 * @brief Sends a single character byte over I2C1
 * @param[in] ch - Byte to send
*/
void I2C1_putchar(unsigned char ch) {
	//force it to wait until TX FIFO is empty
	while (!((I2C1->MASTER.MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK)>= I2C_MFIFOSR_TXFIFOCNT_MINIMUM));

	//transmit a byte
	I2C1->MASTER.MTXDATA = ch;
	
}


/**
 * @brief Send full character string over I2C until limit is reached
 * @param[in] data - String pointer to data to send
 * @param[in] data_size - Amount of bytes to transmit
*/
void I2C1_put(unsigned char *data, uint16_t data_size) {

	//SETS DIR FROM MASTER TO SLAVE
	I2C1->MASTER.MSA |= I2C_MSA_DIR_TRANSMIT;
	//PARAMETERS FOR DATA SEND
	I2C1->MASTER.MCTR |= ((((unsigned int)data_size<<I2C_MCTR_MBLEN_OFS)&I2C_MCTR_MBLEN_MAXIMUM));
	//SET I2C TO ALLOW STOP AND START SEQUENCE
	I2C1->MASTER.MCTR |= (I2C_MCTR_START_ENABLE | I2C_MCTR_STOP_ENABLE);
	//SET BURSTRUN UP AFTER IT HAS BEEN SET UP FOR START AND STOP
	I2C1->MASTER.MCTR |= I2C_MCTR_BURSTRUN_ENABLE;
	//SEND DATA IN LOOP OF EACH CHAR IN data array
	for (int i=0;i<data_size;i++) {
		I2C1_putchar(data[i]);
	}
	//WAIT FOR MSR TO BE IDLE (NOT TRANSMITTING)
	while(!(I2C1->MASTER.MSR & I2C_MSR_IDLE_MASK));
	//DISABLE BURSTRUN
	I2C1->MASTER.MCTR &= ~(I2C_MCTR_MBLEN_MASK | I2C_MCTR_BURSTRUN_ENABLE);

}
