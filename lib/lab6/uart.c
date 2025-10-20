/**
 * ******************************************************************************
 * @file    : uart.c
 * @brief   : UART module header file
 * @details : UART initialization and interaction
 * 
 * @author  : Akshay Narang
 * @date    : 9-5-2025
 * ******************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab6/uart.h"


/**
 * @brief Initialize UART0
*/
void UART0_init() {

	//->check if UART0 module has power enabled
	if (!(UART0->GPRCM.PWREN & UART_PWREN_ENABLE_ENABLE)) {
		//->if here, power was NOT enabled so reset  
		//	set the clear bit, set the key first to unlock write capability, set the assert key
		UART0->GPRCM.RSTCTL |= UART_RSTCTL_RESETSTKYCLR_CLR | UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT;
		//->now enable power
		//	set the key first to unlock write capability and enable power
		UART0->GPRCM.PWREN |= UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE;
	}
	
	//->configure the IOMUX->SECCFG.PINCM for TX and RX
	//Select PINCM10 for use as port I/O -> RX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM22_PF_UART0_RX;
	//Enable input for RX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM22] |= IOMUX_PINCM_INENA_ENABLE;
	///
	//Select PINCM11 for use as port I/O -> TX
	IOMUX->SECCFG.PINCM[IOMUX_PINCM21] |= IOMUX_PINCM_PC_CONNECTED | IOMUX_PINCM21_PF_UART0_TX;
	
	///->UART stuff
	//->set UART BUSCLK
	UART0->CLKSEL |= UART_CLKSEL_BUSCLK_SEL_ENABLE;
	//->set UART clk div for div ratio of 1 for no extra div
	UART0->CLKDIV |= UART_CLKDIV_RATIO_DIV_BY_1;
	//->disbale UART by clearing ENABLE bit of CTL0
	UART0->CTL0 &= ~UART_CTL0_ENABLE_MASK;
	//->set oversampling rate to 16x
	UART0->CTL0 &= ~UART_CTL0_HSE_MASK;
	//->Enable TX,RX,FIFO for UART
	UART0->CTL0 |= UART_CTL0_TXE_ENABLE | UART_CTL0_RXE_ENABLE | UART_CTL0_FEN_ENABLE;
	//->BAUD Rate setup
	//	BRD = UART clock / (oversampling * Baud rate)
	//	=>	32 MHz / (16 * 9600) = 208.33
	//	IBRD = int[BRD] = int[208.33] = 208
	//	FBRD = int[(FRAC[BRD] * 64) + 0.5] = int[(0.33 * 64) + 0.5] = 21
	UART0->IBRD = 208;
	UART0->FBRD = 21;
	//	in order to update the BAUD settings, the UART0->LCRH register must be set up
	//	needs to be set to 8 data bits, 1 stop bit, and no parity bit
	UART0->LCRH |= UART_LCRH_WLEN_DATABIT8;
	//->re-enable UART by setting the ENABLE bit of CT0
	UART0->CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

/**
 * @brief Put a character over UART0
 * @param[in] ch - Character to print
*/
void UART0_putchar(uint8_t ch) {
	//->wait until Transmit FIFO is not full
	while(UART0->STAT & UART_STAT_TXFF_SET) {}//if stuck here, TX FIFO is full
	UART0->TXDATA = ch;//assign char to TX data register
}

/**
 * @brief Retrieve a single character from UART0
*/
char UART0_getchar(void) {
	//->wait until RX FIFO is full
	while((UART0->STAT & UART_STAT_RXFE_SET)){}//while RX queue has nothing
	//return the char from the Receive Data Register and mask it so only the data is returned
	return (UART0->RXDATA & UART_RXDATA_DATA_MASK);//return the value and mask it
}

/**
 * @brief Send a full character string over UART0
 * @param[in] ptr_str - Pointer to the string to print
*/
void UART0_put(char *ptr_str) {
	//check if char that ptr is currently on is NOT null --> meaning NOT end of "string"
	while(*ptr_str != '\0') {
		UART0_putchar(*ptr_str);//put char into putchar repeatedly
		ptr_str++;//increment ptr	
	}
}	
