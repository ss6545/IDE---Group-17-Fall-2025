/******************************************************************************
 * @file    : main.c - for demo 2
 * @brief   : main file for the demo 2 of the car
 * @details : using only weighted average (or COM), go through a figure 8 track
 * 
 * @author	: me
 * @date 	: today
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include "demo2/uart.h"
#include "demo2/timers.h"
#include "demo2/adc12.h"
#include "demo2/camera.h"
#include "demo2/i2c.h"
#include "demo2/oled.h"
#include "demo2/uart_extras.h"


#define CARPET_THRESHOLD			210       // ADC value for detecting carpet (min value above which it is carpet)
#define WHITE_THRESHOLD				2700			// original was 2800
#define BLACK_THRESHOLD				100
#define SERVO_MAX 					10      	// servo 90deg right
#define SERVO_CENTER 				7.7    		// straight
#define SERVO_MIN 					5       	// servo 90deg left
#define SR 							SERVO_MAX-SERVO_CENTER		// for steering
#define MAX_SPEED					50				// duty cycle %
#define ACTUAL_MAX_BRIGHTNESS	3700.0

#define CENTER_PIXEL	63
#define CENTER_RANGE  5

static double steering_wheel;   // holds current percent duty cycle for servo
static double threshold;       // threshold for the lighting of the room
static int carpet_counter;		// for counting number of pixels that are carpet
static int speed;

void read_cam(uint16_t*);
void turn(double);
void stop(void);
void all_wheels_init(void);
int carpet_detected(uint16_t*);
void steer_center(void);
void go_straight(void);
void speed_both_forward(int duty_cycle);
void delay(void);
double mapRange(double n, double start1, double stop1, double start2, double stop2, double center2);
float data_update(float x);
/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void delay() {
	volatile long timer = 0;
	while (timer < 40000){
		timer+=1;
	}
}

unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

int main() {
	
	UART0_init();
	ADC0_init();
	Camera_init();
	OLED_Init();
	
	// all wheels are intialized to be straight and not moving
	all_wheels_init();
	delay();
	uint16_t* cameraData_ptr;
	//uint16_t* avgData;
	while(1) {
		//right
		// TIMA0_PWM_DutyCycle(0,...) - is for forward
		// TIMA0_PWM_DutyCycle(1,...) - is for backward
		
		
		//left
		// TIMA0_PWM_DutyCycle(2,...) - is for forward
		// TIMA0_PWM_DutyCycle(3,...) - is for backward
		//holds cameraData pointer
		if (Camera_isDataReady()) {
			//TIMA1_PWM_DutyCycle(3200,10);
			
			cameraData_ptr = Camera_getData();
//			for (int i=0;i<128;i++) {
//			
//				avgData[i] = (uint16_t)(data_update(cameraData_ptr[i]));
//			
//			}
			
			OLED_DisplayCameraData(cameraData_ptr);
			
			go_straight();
			
			//->output camera data
//			UART0_put("-1 ");//start
//			
//			for (int i = 0; i < 128; i++) {
//				//update stringHolder's first element w the camera data on each iteration
//				//snprintf(stringHolder, sizeof(stringHolder),"%u\r\n",cameraData_ptr[i]);
//				//UART0_put(stringHolder);//output the current version of stringHolder
//				UART0_printDec(cameraData_ptr[i]);
//				UART0_put(" ");
//			}
//		
//			UART0_put("-2\r\n");//stop
//			
			read_cam(cameraData_ptr);
		}
	}

	return 0;

}// end main


void read_cam(uint16_t* cameraData_ptr){
	int sum_adc_values=0;
	int weighted_sum_adc_values=0;
	double brightest_position_com;		// stores current brightest pixel

	if(carpet_detected(cameraData_ptr)){
        // stop motors
        stop();
		steer_center();
    } // end carpet

    else {
			TIMA1_PWM_init(3200,199,10);
		for (int i = 0; i<128; i++) {
			sum_adc_values += cameraData_ptr[i];
			weighted_sum_adc_values += i * cameraData_ptr[i];
		}

		brightest_position_com = (double)weighted_sum_adc_values/(double)sum_adc_values;	// calculates the pixel number that is the brightest

		// (center(+-range)) of pixels that can be considered as center
		double mappedCenterOfMass = mapRange(brightest_position_com, 64 + CENTER_RANGE, 64 - CENTER_RANGE, SERVO_MIN, SERVO_MAX, SERVO_CENTER);
		turn(mappedCenterOfMass);
//		if(brightest_position_com >= (CENTER_PIXEL-center_range) && brightest_position_com <= (CENTER_PIXEL+center_range)) {	// if Center of mass is within [63+-10], center of mass is within center range
//			go_straight();
//			steer_center();
//		}
//		else {	// for any bright spot not within center range
//			double diff = brightest_position_com - CENTER_PIXEL;
//			speed_both_forward(30);	// run DC motors forward at 30% duty cycle
//			turn(diff); 			// turn according to difference
//		}
	}
}




void turn(double diff){
	if(diff>10){
		steering_wheel = 10;
	}
	else if(diff<5){
		steering_wheel=5;
	}
	else {
		steering_wheel = diff;
	}
	//steering_wheel = SERVO_CENTER + SR*(diff/CENTER_PIXEL); // turn to right or left (+ or -) based on how far we are from the middle (if we are at the middle, 2nd term = 0)
  speed_both_forward(30);
	TIMA1_PWM_DutyCycle(3200, steering_wheel);
	
}

void go_straight(){
	speed = 35;
	TIMA0_PWM_DutyCycle(0,3200,speed);		// right go forward at 27%
	TIMA0_PWM_DutyCycle(1,3200,0);
	
	TIMA0_PWM_DutyCycle(2,3200,speed);		// left go forward at 27%
	TIMA0_PWM_DutyCycle(3,3200,0);
	
	TIMA1_PWM_DutyCycle(3200, SERVO_CENTER);		// steer to center
}

void speed_both_forward(int duty_cycle){
	TIMA0_PWM_DutyCycle(0,3200,duty_cycle);		// right go forward
	TIMA0_PWM_DutyCycle(1,3200,0);
	
	TIMA0_PWM_DutyCycle(2,3200,duty_cycle);		// left go forward
	TIMA0_PWM_DutyCycle(3,3200,0);
}

void stop(){
	TIMA0_PWM_DutyCycle(0,3200,0);  // stop right forward
  	TIMA0_PWM_DutyCycle(1,3200,0);  // stop right backward
  	TIMA0_PWM_DutyCycle(2,3200,0);  // stop left forward
  	TIMA0_PWM_DutyCycle(3,3200,0);  // stop left backward
}

void all_wheels_init(){
	TIMA0_PWM_init(0,3200,0,0);
	TIMA0_PWM_init(1,3200,0,0);
	TIMA0_PWM_init(2,3200,0,0);
	TIMA0_PWM_init(3,3200,0,0);
	TIMA1_PWM_init(3200,199,SERVO_CENTER);
}

void steer_center(){
	TIMA1_PWM_DutyCycle(3200, SERVO_CENTER);
}

int carpet_detected(uint16_t* cameraData_ptr){
	carpet_counter=0;
	for (int i = 0; i<128; i++) {
        if(cameraData_ptr[i]> CARPET_THRESHOLD && cameraData_ptr[i]<WHITE_THRESHOLD){
			carpet_counter++;
		}
  }

	if(carpet_counter>=120) {
		return 1;
	}
	return 0;
}

double mapRange(double n, double start1, double stop1, double start2, double stop2, double center2) {

//    double center = 63.0;
//    double deadband = 10.0;

    // Exact straight zone
    if (n >= CENTER_PIXEL - CENTER_RANGE && n <= CENTER_PIXEL + CENTER_RANGE) {
        return center2;  // perfectly straight
    }

    // Outside deadband: linear ramp to left (10) or right (5)
    if (n > CENTER_PIXEL + CENTER_RANGE) {	// go right
        double t = (n - (CENTER_PIXEL)) / (3*CENTER_RANGE);
        return (7.5 - 2.5 * t);
    } else {		// go left
        double t = (CENTER_PIXEL-n) / (3*CENTER_RANGE);
        return (7.5 + 2.5 * t);   // 5->7.5
    }

}


/*
    5-sample Weighted Moving Average (WMA) for bpm update
    Smooths out the bpm curve based on a weighted average of the last 5 entries
*/
//float buffer[5] = {0};
//uint8_t bi = 0; // buffer index

//float data_update(float x)
//{
//    buffer[bi] = x;
//    bi = (bi + 1) % 5;

//    // newest = buffer[(bi + 4) % 5], then older
//    float b0 = buffer[(bi + 4) % 5];  // newest
//    float b1 = buffer[(bi + 3) % 5];
//    float b2 = buffer[(bi + 2) % 5];
//    float b3 = buffer[(bi + 1) % 5];
//    float b4 = buffer[(bi + 0) % 5];  // oldest

//    // Example weights (must sum to 1.0):
//    // 0.40, 0.25, 0.20, 0.10, 0.05  — tune as needed
//    return (0.2f * b0 +
//            0.2f * b1 +
//            0.2f * b2 +
//            0.2f * b3 +
//            0.2f * b4);
//}


