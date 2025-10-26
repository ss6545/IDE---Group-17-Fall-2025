#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include <math.h>
#include "lab5/adc12.h"
#include "lab5/timers.h"
#include "lab5/uart.h"
#include "uart_extras.h"

void TIMG6_IRQHandler(void);

int main(){
	
	UART0_init();
	ADC0_init();
	//TIMG6 w 2 Hz freq
	TIMG6_init(20000, 99, GPTIMER_CLKDIV_RATIO_DIV_BY_8);
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;	// start (enable) TIMG6 timer
	while (1) {}

	return 0;
}

void TIMG6_IRQHandler(void) {
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK; // clear the interrupt that caused the calling of this function (the zero event interrupt)
	
//	// For CdS cell
//	int i = ADC0_getVal();
//	UART0_put("ADC value read: \r\n\0");
//	UART0_put("In decimal= \0");
//	UART0_printDec(i);
//	UART0_put("\r\nIn hex    = 0x\0");
//	UART0_printHex(i);
//	UART0_put("\r\n\r\n\0");
	
	// For TMP36
	double c = 100 * (3.3*((double)ADC0_getVal()/4095.0 - 0.5/4096.0) - 0.5); 	// temperature in °C		
	double f = c * 9.0/5.0 + 32;	// °F = (°C × 9/5) + 32
	UART0_put("Temperature value: \r\n\0");
	UART0_put("In Celsius    = \0");
	UART0_printFloat(c);
	UART0_put(" C\r\nIn Fahrenheit = \0");
	UART0_printFloat(f);
	UART0_put(" F\r\n\r\n\0");
}
