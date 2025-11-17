#include <ti/devices/msp/msp.h>
#include <stdint.h>
#include "lab5/adc12.h"
#include "lab5/timers.h"
#include "lab5/uart.h"
#include "uart_extras.h"

#define ADC_L_TH  1000    // 1.5V
#define ADC_H_TH  2400    // 2.0V

volatile uint16_t ADC_inp = 0;
volatile uint32_t new_time_ms = 0;
volatile uint32_t old_time_ms = 0;

float adc_filtered = 0.0f;
uint8_t is_high = 0;

float bpm = 0;
float bpm_filtered = 0;

int peak_counter = 0;

// declaring function prototype
float bpm_update(float x);

int main() 
{
    UART0_init();
    ADC0_init();

    // 1 kHz interrupt (1 ms)
    TIMG6_init(3200, 9, GPTIMER_CLKDIV_RATIO_DIV_BY_1);
    TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_MASK;

    UART0_put("Heartbeat monitor\n\r");

    while (1) { }
    return 0;
}

/*
	This timer is used for ADC input handling, as well as output on putty every 6 seconds
*/
void TIMG6_IRQHandler(void)
{
    TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_MASK;
    
    new_time_ms++;

    ADC_inp = ADC0_getVal();

    // low-pass filter
    adc_filtered = 0.8f * adc_filtered + 0.2f * ADC_inp;

    if ((adc_filtered > ADC_H_TH) && (!is_high))
    {
        uint32_t dt = new_time_ms - old_time_ms;

        // valid beat interval
        if (dt > 200 && dt < 2000)
        {
            bpm = 60000.0f / dt;   // ms -> bpm
            bpm_filtered = bpm_update(bpm);
            peak_counter++;
        }

        old_time_ms = new_time_ms;
        is_high = 1;
    }
    else if (adc_filtered < ADC_L_TH)
    {
        is_high = 0;
    }

    // printing 6000 ms (6 sec)
    if (new_time_ms % 6000 == 0)
    {
        UART0_put("\n\rHeart Rate = ");
        UART0_printFloat(bpm_filtered);
        UART0_put(" BPM\r\n");
    }
}


/*
	3-sample Weighted moving average (WMA) for bpm update
	Smooths out the bpm curve based on a weighted average of the last 3 entries
*/
float buffer[3] = {0};
uint8_t bi = 0;

float bpm_update(float x)
{
    buffer[bi] = x;
    bi = (bi + 1) % 3;

    // newest at buffer[(index+2)%3], then older
    float b0 = buffer[(bi + 2) % 3];
    float b1 = buffer[(bi + 1) % 3];
    float b2 = buffer[(bi + 0) % 3];

    return (0.6f * b0 + 0.3f * b1 + 0.1f * b2); // weighted average: recentmost entry gets weighted more
}
