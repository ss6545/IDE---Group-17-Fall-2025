/*
* This file is for the sanity of the coder - keeping all constants and variables for the car in one place
*/

#ifndef ALL_H
#define ALL_H

/*!!!!!!!!!!!!!!!!!!!!!!!!!!! CONSTANTS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
// thresholds
#define CARPET_THRESHOLD			350             // was 210 ADC value for detecting carpet (min value above which it is carpet)
#define WHITE_THRESHOLD				2000			// original was 2800 // LOWER THIS MORE!!!!!
#define BLACK_THRESHOLD				100

// servo (steering)
#define SERVO_CENTER 					7.05    		// straight
#define SERVO_MAX 						SERVO_CENTER+2.5      	// servo fully left (not 90 deg definitely, more like 30 deg)
#define SERVO_MIN 						SERVO_CENTER-2.5       	// servo fully right (not 90 deg)
#define SR 							SERVO_MAX-SERVO_CENTER		// for steering

// DC motors (speed)
#define MAX_SPEED				35				// duty cycle %
#define MIN_SPEED				30

// camera stuff
#define ACTUAL_MAX_BRIGHTNESS		3700.0
#define CENTER_PIXEL	        	63
#define CENTER_RANGE            5

// PID stuff
#define MAX_INTEGRAL            10      // this will make "integral" term have a contribution of 1 for ki=0.01



/*!!!!!!!!!!!!!!!!!!!!!!!!!!! VARIABLES !!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

extern double steering_wheel;   // holds current percent duty cycle for servo
// extern double threshold;       // threshold for the lighting of the room
extern int carpet_counter;		// for counting number of pixels that are carpet
extern int speed;

// PID stuff
extern double error;
extern double old_error;
extern double integral;		// accumulates the integral errors so that we can take care of intgral windup
extern double kp;
extern double ki;
extern double kd;

#endif
