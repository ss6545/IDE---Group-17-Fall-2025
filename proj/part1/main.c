/******************************************************************************
Akshay Narang 
IDE Lab05 - 
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "lab5/leds.h"
#include "lab5/switches.h"


/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void keepLEDon(void) {

	volatile long timer = 0;
	while (timer < 2000000) timer+=1;
	
}

/*main makes the LEDs (outputs) light up according to the inputs of?the?switches*/
int main() {
	//->set up LED1 and S1
	LED1_init();
	S1_init();
	LED2_init();
	S2_init();
	
	
	int numS2presses = 0;
	
	
	while (1) {
		
		if (S1_pressed()) {
			TIMG6_init();
			
			
			
			
			
			
			
			
			numS2presses += 1;
			if (numS2presses == 4) numS2presses = 1;//if past 3rd press -> set back to 1st color cycle
			
			switch (numS2presses) {
				case 1:
					LED2_set(1);//red
					keepLEDon();
					LED2_set(0);//all off
					LED2_set(2);//green
					keepLEDon();
					LED2_set(0);//all off
					LED2_set(3);//blue
					keepLEDon();
					LED2_set(0);//all off
					break;
				case 2:
					LED2_set(4);//cyan
					keepLEDon();
					LED2_set(0);//all off
					LED2_set(5);//magenta
					keepLEDon();
					LED2_set(0);//all off
					LED2_set(6);//yellow
					keepLEDon();
					LED2_set(0);//all off
					break;
				case 3:
					LED2_set(7);//white
					keepLEDon();
					LED2_set(0);//all off
					break;
				default:
				break;
			
			}
				
		}
		
		
		//->check if S1 was pressed, 
		if (S1_pressed()) {
				LED1_set(1);
				continue;
			}
			LED1_set(0);

		}	
		
	

	
	
return 0;

}
