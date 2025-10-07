/**
 * ******************************************************************************
 * @file    : uart.c
 * @brief   : UART module file
 * @details : UART initialization and interaction
 * 
 * @author 	: Sahima Sachdev
 * @date 		: 9/5/2025
 * ******************************************************************************
*/
#include <ti/devices/msp/msp.h>
#include "../lab2/uart.h"
#include "../lib/sysctl.h"
/**
 * @brief Initialize UART0
*/
void UART0_init(){
	if(!(UART0->GPRCM.PWREN & UART_PWREN_ENABLE_ENABLE)) { // check if the PWREN is enabled or not
		UART0->GPRCM.RSTCTL = UART_RSTCTL_RESETSTKYCLR_CLR | UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT; // clear, unlock and assert reset for the RSTCTL register
		UART0->GPRCM.PWREN = UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE; // unlock the PWREN and enable it
	}
	// Select UART0_RX for use as port I/O
	IOMUX -> SECCFG.PINCM[IOMUX_PINCM22] |= (IOMUX_PINCM_PC_CONNECTED|IOMUX_PINCM22_PF_UART0_RX);
	// Select UART0_TX for use as port I/O
	IOMUX -> SECCFG.PINCM[IOMUX_PINCM21] |= (IOMUX_PINCM_PC_CONNECTED|IOMUX_PINCM21_PF_UART0_TX);
	// Enable input on UART0_RX
	IOMUX -> SECCFG.PINCM [IOMUX_PINCM22] |= IOMUX_PINCM_INENA_ENABLE;
	// Enable the Bus clock for the UART0
	UART0 -> CLKSEL = UART_CLKSEL_BUSCLK_SEL_ENABLE;
	// Set the clock divisions to 1
	UART0 -> CLKDIV |= UART_CLKDIV_RATIO_DIV_BY_1;
	// Disable the CTL0 bit to program the control registers
	UART0 -> CTL0 &= ~UART_CTL0_ENABLE_MASK; // clearing this bit
	// UART0 -> CTL0 |= UART_CTL0_ENABLE_DISABLE; // disabling it
	// Set oversampling to 16x
	UART0 -> CTL0 &= ~UART_CTL0_HSE_MASK;
	// Enable transmit on UART0
	UART0 -> CTL0 |= UART_CTL0_TXE_ENABLE;
	// Enable receive on UART0
	UART0 -> CTL0 |= UART_CTL0_RXE_ENABLE;
	// Enable FIFO
	UART0 -> CTL0 |= UART_CTL0_FEN_ENABLE;
	// BRD = UART Clock / (Oversampling x Baud rate)
  // UART clock rate = 32MHz, oversampling rate = 16x, baud rate=9600	
	// initializing the integer part of BRD
	//UART0 -> IBRD |= (uint16_t)(SYSCTL_SYSCLK_getULPCLK()/(16*9600));
	UART0 -> IBRD = 208;
	// initializing the fractional part of BRD
	//UART0 -> FBRD |= (SYSCTL_SYSCLK_getULPCLK()/(16*9600))%64;
	UART0 -> FBRD = 21;
	// setting UART to use 8 data bits, 1 stop bit, and no parity bit
	// UART0 -> LCRH &= ~UART_LCRH_PEN_MASK & ~UART_LCRH_STP2_MASK; // clearing the required bits
	UART0 -> LCRH = UART_LCRH_WLEN_DATABIT8;
	// Re-enable the CTL0 bit to program the control registers
	UART0 -> CTL0 |= UART_CTL0_ENABLE_ENABLE;
}

void UART1_init(){
	if(!(UART1->GPRCM.PWREN & UART_PWREN_ENABLE_ENABLE)) { // check if the PWREN is enabled or not
		UART1->GPRCM.RSTCTL = UART_RSTCTL_RESETSTKYCLR_CLR | UART_RSTCTL_KEY_UNLOCK_W | UART_RSTCTL_RESETASSERT_ASSERT; // clear, unlock and assert reset for the RSTCTL register
		UART1->GPRCM.PWREN = UART_PWREN_KEY_UNLOCK_W | UART_PWREN_ENABLE_ENABLE; // unlock the PWREN and enable it
	}
	// Select UART1_RX for use as port I/O
	IOMUX -> SECCFG.PINCM[IOMUX_PINCM20] |= (IOMUX_PINCM_PC_CONNECTED|IOMUX_PINCM20_PF_UART1_RX);
	// Select UART1_TX for use as port I/O
	IOMUX -> SECCFG.PINCM[IOMUX_PINCM19] |= (IOMUX_PINCM_PC_CONNECTED|IOMUX_PINCM19_PF_UART1_TX);
	// Enable input on UART1_RX
	IOMUX -> SECCFG.PINCM [IOMUX_PINCM20] |= IOMUX_PINCM_INENA_ENABLE;
	// Enable the Bus clock for the UART1
	UART1 -> CLKSEL = UART_CLKSEL_BUSCLK_SEL_ENABLE;
	// Set the clock divisions to 1
	UART1 -> CLKDIV |= UART_CLKDIV_RATIO_DIV_BY_1;
	// Disable the CTL0 bit to program the control registers
	UART1 -> CTL0 &= ~UART_CTL0_ENABLE_MASK; // clearing this bit
	// UART1 -> CTL0 |= UART_CTL0_ENABLE_DISABLE; // disabling it
	// Set oversampling to 16x
	UART1 -> CTL0 &= ~UART_CTL0_HSE_MASK;
	// Enable transmit on UART1
	UART1 -> CTL0 |= UART_CTL0_TXE_ENABLE;
	// Enable receive on UART1
	UART1 -> CTL0 |= UART_CTL0_RXE_ENABLE;
	// Enable FIFO
	UART1 -> CTL0 |= UART_CTL0_FEN_ENABLE;
	// BRD = UART Clock / (Oversampling x Baud rate)
  // UART clock rate = 32MHz, oversampling rate = 16x, baud rate=9600	
	// initializing the integer part of BRD
	//UART1 -> IBRD |= (uint16_t)(SYSCTL_SYSCLK_getULPCLK()/(16*9600));
	UART1 -> IBRD = 208;
	// initializing the fractional part of BRD
	//UART1 -> FBRD |= (SYSCTL_SYSCLK_getULPCLK()/(16*9600))%64;
	UART1 -> FBRD = 21;
	// setting UART to use 8 data bits, 1 stop bit, and no parity bit
	// UART1 -> LCRH &= ~UART_LCRH_PEN_MASK & ~UART_LCRH_STP2_MASK; // clearing the required bits
	UART1 -> LCRH = UART_LCRH_WLEN_DATABIT8;
	// Re-enable the CTL0 bit to program the control registers
	UART1 -> CTL0 |= UART_CTL0_ENABLE_ENABLE;
}


/**
 * @brief Put a character over UART0
 * @param[in] ch - Character to print
*/
void UART0_putchar(char ch){
	while( UART0 -> STAT & UART_STAT_TXFF_SET) { // While the transmit queue is full
		//loop
	}
	//come out of the loop when the transmit queue is empty
	// UART0 -> TXDATA &= ~UART_TXDATA_DATA_MASK; // clear anything that might be there in the transmit queue
	UART0 -> TXDATA =	ch; // assign the data to the transmit queue
}


/**
 * @brief Retrieve a single character from UART0
*/
char UART0_getchar(){
	while ((UART0 -> STAT & UART_STAT_RXFE_SET)){ // While the receive queue is empty
	}
	// when receive queue is full
	return UART0 -> RXDATA & UART_RXDATA_DATA_MASK; //return the data value in RX queue
}

/**
 * @brief Retrieve a single character from UART0
 * NOTE: NEED TO USE UART0_isDataAvailable() BEFORE USING THIS FUNCTION
*/
char UART0_getcharfast(){
	return UART0 -> RXDATA & UART_RXDATA_DATA_MASK; //return the data value in RX queue
}


/**
 * @brief Send a full character string over UART0
 * @param[in] ptr_str - Pointer to the string to print
*/
void UART0_put(char *ptr_str){
	while(*ptr_str != '\0'){ // check for null termination
		UART0_putchar(*ptr_str); // repeatedly calling putchar
		ptr_str++; // increase pointer to next char
	}
}



/**
 * @brief Put a character over UART1
 * @param[in] ch - Character to print
*/
void UART1_putchar(char ch){
	while( UART1 -> STAT & UART_STAT_TXFF_SET) { // While the transmit queue is full
		//loop
	}
	//come out of the loop when the transmit queue is empty
	// UART1 -> TXDATA &= ~UART_TXDATA_DATA_MASK; // clear anything that might be there in the transmit queue
	UART1 -> TXDATA =	ch; // assign the data to the transmit queue
}


/**
 * @brief Retrieve a single character from UART1
*/
char UART1_getchar(){
	while ((UART1 -> STAT & UART_STAT_RXFE_SET)){ // While the receive queue is empty
	}
	// when receive queue is full
	return UART1 -> RXDATA & UART_RXDATA_DATA_MASK; //return the data value in RX queue
}

/**
 * @brief Retrieve a single character from UART1
 * NOTE: NEED TO USE UART1_isDataAvailable() BEFORE USING THIS FUNCTION
*/
char UART1_getcharfast(){
	return UART1 -> RXDATA & UART_RXDATA_DATA_MASK; //return the data value in RX queue
}


/**
 * @brief Send a full character string over UART1
 * @param[in] ptr_str - Pointer to the string to print
*/
void UART1_put(char *ptr_str){
	while(*ptr_str != '\0'){ // check for null termination
		UART1_putchar(*ptr_str); // repeatedly calling putchar
		ptr_str++; // increase pointer to next char
	}
}

int UART0_isDataAvailable(){
	int go = 0 ; // boolean = false
	// if a character is available , set go = TRUE
	go = !(UART0 -> STAT & UART_STAT_RXFE_SET);				// UART0 -> STAT & UART_STAT_RXFE_SET = 0 when receive queue is full and ready to read from
	return go;
}
	
int UART1_isDataAvailable(){
	int go = 0 ; // boolean = false
	// if a character is available , set go = TRUE
	go = !(UART1 -> STAT & UART_STAT_RXFE_SET);				// UART1 -> STAT & UART_STAT_RXFE_SET = 0 when receive queue is full and ready to read from
	return go;
}







	