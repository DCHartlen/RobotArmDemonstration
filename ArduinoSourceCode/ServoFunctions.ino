int ActuateServo(ServoObj &servo, float proposedAngle){
    // Set the servo's angle while respecting bounds
    int tempPwm = 0;    // Temporary servo raw. must respect bounds
    //Check servo bounds, set temporary raw value, return error code
    tempPwm = InterpolateServo(proposedAngle, servo);

    // Set current pwm and angle based on interpolation
    servo.currentPwm = tempPwm;
    servo.currentAngle = proposedAngle;
    // Set servo pwm to move servo
    servo.ServoControl.writeMicroseconds(servo.currentPwm);

    return 0;
}

int InterpolateServo(float &inputAngle, ServoObj servo) {
    /*
    This interpolation function computes PWM given an input angle and the
    calibration points in the servo object. If the PWM bounds are exceeded,
    it also limits the input angle via a pointer. 
    Returns the output pwm to move the servo.
    */
    int pwmOut;
    double temp;
    // Linearly interpolate PWM based on inputAngle and calibration points
    pwmOut = (inputAngle-servo.calAngle1)*(servo.calPwm2-servo.calPwm1)/
                (servo.calAngle2-servo.calAngle1)+servo.calPwm1;
    // check if PWM bounds are exceeded
    if ((pwmOut >= servo.UppPwmBound) || (pwmOut <= servo.lwrPwmBound)){
        // Constrain PWM to bounds
        pwmOut = constrain(pwmOut, servo.lwrPwmBound, servo.UppPwmBound);
        // Using constrained PWM, alter inputAngle to respect bounds based on
        // Calibration points.
        inputAngle = (float(pwmOut)-servo.calPwm1)*(servo.calAngle2-servo.calAngle1)/
                (servo.calPwm2-servo.calPwm1)+servo.calAngle1;
    }
    // Return the interpolated and bounded PWM for servo.
    return pwmOut;
}