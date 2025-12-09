#include "pid.h"
#include "all.h"
#include <math.h>

// // main function that coordinates all other functions in pid
// void steer_with_PID(double pixel_act){

//     // only detect error if it is large enough
//     if(pixel_act > CENTER_PIXEL-CENTER_RANGE && pixel_act < CENTER_PIXEL+CENTER_RANGE) {
//         error = 0;
//     }
//     else {
//         error = CENTER_PIXEL - pixel_act;
//     }

//     // ******gives uncut value of PID result of steering
//     double steering_wheel_local = pidSteeringValue(error);
//     // ******cut stuff out
//     // take care of center range for integral update
//     // try extern int integral in pid.h and then int integral = 0 in pid.c, then include pid.h in main.c and u can change smoothly
//     steering_wheel = clipAndSave(steering_wheel_local, error);

// }

// // returns the un-cut value that needs to be updated in turn()
// double pidSteeringValue(double error_local) {
	
//   double integral_local = integral + error_local;
//   // new values add onto the old steering value
//   return steering_wheel_old + (Kp*error_local) + (Ki*integral_local) + (Kd*(error_local - error_old1));
// 	//return (Kp*error_local) + (Ki*integral_local) + (Kd*(error_local - error_old1));

// }

// // clip both the integral and the steering_wheel, save resulting error, integral and steering_wheel
// double clipAndSave(double steering_wheel_local, double error_local){
//     if(steering_wheel_local>SERVO_MAX){  // if PID demands too much right turn
// 		steering_wheel_local = SERVO_MAX;    // clip at max limit
// 	}
// 	else if(steering_wheel_local<SERVO_MIN){  // if PID demands too much left
// 		steering_wheel_local = SERVO_MIN;   // clip at min limit
// 	}
// 	else if(fabs(error_local) >= 5){
// 		steering_wheel = steering_wheel_local;  // let pid do what it was doing
// 		integral += error_local;    // inegral error is calculated
// 	}

//     error = error_local;
//     error_old1 = error;   // error_old1 = error
//     steering_wheel_old = steering_wheel;    // save for next time

//     return steering_wheel;  // update
// }   


// need to stay here
static double P = 0;
static double I = 0;
static double D = 0;

double steer_with_PID(double servo_act){

    error = servo_act - SERVO_CENTER;

    // ***************calculate proportional value
    P = kp*error;

    // ***************calculate integral value
    integral += error;
    // prevent windup
    if(integral > MAX_INTEGRAL) {
        integral = MAX_INTEGRAL;
    } 
    else if (integral < -MAX_INTEGRAL) {
        integral = -MAX_INTEGRAL;
    }
    I = ki*integral;

    // ***************calculate derivative value
    D = kd*(error - old_error);

    // save stuff for next time
    old_error = error;

    // we are done
    return P+I+D;       // this answer is supposed to be in terms of servo duty cycle
}
