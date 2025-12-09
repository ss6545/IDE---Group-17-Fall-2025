#ifndef PID_H
#define PID_H


// main function that coordinates all other functions in pid
double steer_with_PID(double pixel_act);

// returns the un-cut value that needs to be updated in turn()
double pidSteeringValue(double error_local);

// clip both the integral and the steering_wheel, save resulting error, integral and steering_wheel
double clipAndSave(double steering_wheel_local, double error_local);

// ---------- helper: map PID pixels -> servo units ----------
double pixels_to_servo(double pid_pixels);


#endif
