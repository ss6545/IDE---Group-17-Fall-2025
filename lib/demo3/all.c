#include "all.h"

double steering_wheel = 0;   // holds current percent duty cycle for servo
// extern double threshold;       // threshold for the lighting of the room
int carpet_counter = 0;		// for counting number of pixels that are carpet
int speed = 0;

// PID stuff
double error = 0;
double old_error = 0;
double integral = 0;		// accumulates the integral errors so that we can take care of intgral windup
// give actual values to these three
double kp = 1.15;		// 6 worked for old value of carpet threshold		// 1.9 worked for max_speed = 32		// 0.6 working-ish for MAX_SPEED=40
double ki = 0.001;
double kd = 0.45;		// 0.2 worked for old value of carpet threshold



/* Trial 1
for MAX_SPEED = 40, MIN_SPEED = 35,
kp = 1.15, ki = 0, kd = 0.35 worked decent, but need ki
*/

/* Trial 2
for MAX_SPEED = 40, MIN_SPEED = 35,
kp = 1.15, ki = 0.01, kd = 0.4 workeds most of the time, but sometimes hits the wall at sharp turns
*/
