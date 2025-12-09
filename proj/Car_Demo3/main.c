/******************************************************************************
 * @file    : main.c
 * @brief   : main file for the final car race!!!
 * @details : using everything, including PID
 * 
 * @author	: me
 * @date 		: today
******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include <math.h>
#include "demo3/uart.h"
#include "demo3/timers.h"
#include "demo3/adc12.h"
#include "demo3/camera.h"
#include "demo3/i2c.h"
#include "demo3/oled.h"
#include "demo3/uart_extras.h"

#include "demo3/all.h"		// needed for all the constants and variables
#include "demo3/pid.h"

// function prototypes
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
double find_brightest_pixel(uint16_t* cameraData_ptr);
// just turn, no clip
void PID_turn(void);
int turnSpeed(void);

/*
brief: keeps LEDs on in order to cycle through colors for LED2rgb with S2 input
note: only used in main.c
*/
void delay() { // delays for 10ms
	volatile long timer = 0;
	while (timer < 40000){
		timer+=1;
	}
}

unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

//uint16_t* cameraData_ptr;

    //right
	// TIMA0_PWM_DutyCycle(0,...) - is for forward
	// TIMA0_PWM_DutyCycle(1,...) - is for backward
		
		
	//left
	// TIMA0_PWM_DutyCycle(2,...) - is for forward
	// TIMA0_PWM_DutyCycle(3,...) - is for backward
	//holds cameraData pointer
    
int main(){
    // initializing everything
    UART0_init();
	ADC0_init();
	Camera_init();
	//OLED_Init();

	// all wheels are intialized to be straight and not moving
	all_wheels_init();
    
//    // array holding all 128 pixels of camera data
//	uint16_t* cameraData_ptr;

    while(1){
        if (Camera_isDataReady()) {		
						// array holding all 128 pixels of camera data
            uint16_t* cameraData_ptr = Camera_getData();
            //go_straight();
						read_cam(cameraData_ptr);
						//OLED_DisplayCameraData(cameraData_ptr);

//          ->output camera data
//          UART0_put("-1 ");//start
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
        }
    }
    return 0;
}

void read_cam(uint16_t* cameraData_ptr){
	double brightest_position_com;		// stores current brightest pixel

//	if(carpet_detected(cameraData_ptr)){
//    // stop motors
//    stop();
//		steer_center();
//  } // end carpet

//  else {
		// find the brigthest spot
		brightest_position_com = find_brightest_pixel(cameraData_ptr);
		double actual = mapRange(brightest_position_com, 64 + CENTER_RANGE, 64 - CENTER_RANGE, SERVO_MIN, SERVO_MAX, SERVO_CENTER);
		double correction = steer_with_PID(actual);
		turn(SERVO_CENTER + correction);
//  }
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


double find_brightest_pixel(uint16_t* cameraData_ptr){
    int sum_adc_values=0;
	int weighted_sum_adc_values=0;

    for (int i = 0; i<128; i++) {
		sum_adc_values += cameraData_ptr[i];
		weighted_sum_adc_values += i * cameraData_ptr[i];
	}
    return (double)weighted_sum_adc_values/(double)sum_adc_values;	// calculates the pixel number that is the brightest
}



// change to return something based on whether we are under or over turning
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
  speed_both_forward(turnSpeed());
	TIMA1_PWM_DutyCycle(3200, steering_wheel);
	
}

// for determining what speed it should go while steering
int turnSpeed() {

	double decSpeed = (steering_wheel - SERVO_CENTER)/2.5;
	double abs_decSpeed = (decSpeed < 0) ? -decSpeed : decSpeed;
	
	double percentCalc = (1-abs_decSpeed);
	
	int newSpeed = (int)(MIN_SPEED + (percentCalc*(MAX_SPEED-MIN_SPEED)));
	
	return newSpeed;

} 


// just turn, no clip
void PID_turn(){
    speed_both_forward(speed);
		TIMA1_PWM_DutyCycle(3200, steering_wheel);
}

void go_straight(){
	speed = MAX_SPEED;
	TIMA0_PWM_DutyCycle(0,3200,speed);		// right go forward at MAX_SPEED
	TIMA0_PWM_DutyCycle(1,3200,0);
	
	TIMA0_PWM_DutyCycle(2,3200,speed);		// left go forward at MAX_SPEED
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


double mapRange(double n, double start1, double stop1, double start2, double stop2, double center2) {

//    double center = 63.0;
//    double deadband = 10.0;

    // Exact straight zone
    if (n >= CENTER_PIXEL - CENTER_RANGE && n <= CENTER_PIXEL + CENTER_RANGE) {
        return SERVO_CENTER;  // perfectly straight
    }

    // Outside deadband: linear ramp to left (10) or right (5)
    if (n > CENTER_PIXEL + CENTER_RANGE) {	// go right
        double t = (n - (CENTER_PIXEL)) / (3*CENTER_RANGE);
        return (SERVO_CENTER - 2.5 * t);
    } else {		// go left
        double t = (CENTER_PIXEL-n) / (3*CENTER_RANGE);
        return (SERVO_CENTER + 2.5 * t);   // 5->7.5
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
//    // 0.40, 0.25, 0.20, 0.10, 0.05  ? tune as needed
//    return (0.2f * b0 +
//            0.2f * b1 +
//            0.2f * b2 +
//            0.2f * b3 +
//            0.2f * b4);
//}
