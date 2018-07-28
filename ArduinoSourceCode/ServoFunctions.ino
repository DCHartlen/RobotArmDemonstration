int ActuateServo(ServoObj servo, float proposedAngle){
    // Set the servo's angle while respecting bounds
    int tempPwm = 0;    // Temporary servo raw. must respect bounds
    int errorCode;  // Error code returned by bound check
    //Check servo bounds, set temporary raw value, return error code
    errorCode = ServoBoundCheck(servo, proposedAngle,tempPwm);

    //Based on error code, write movement that servo will make
    switch(errorCode) {
        // No error code: set servo to proposed angle
        default:
            servo.currentPwm = tempPwm;
            servo.currentAngle = proposedAngle;
            break;
        // Error 1: max bound exceeded. Set to maximum angle
        case 1:
            servo.currentPwm = servo.UppPwmBound;
            servo.currentAngle = InterpolationFloat(servo.calAngle1,servo.calAngle2,servo.calPwm1,
                                                     servo.calPwm2, servo.UppPwmBound);
            break;
        // Error -1: minimum bound exceeded. Set to minimum angle.  
        case -1:
            servo.currentPwm = servo.lwrPwmBound;
            servo.currentAngle = InterpolationFloat(servo.calAngle1,servo.calAngle2,servo.calPwm1,
                                                     servo.calPwm2, servo.lwrPwmBound);
            break;
    }
    // Based on movement saved above, move the actual servo. 
    servo.ServoControl.writeMicroseconds(servo.currentPwm);
    return errorCode;   //return the error code
}

int ServoBoundCheck(ServoObj servo, float servoAngle, int &servoPwmValue){
    // Error codes: 1: maximum bound exceeded, -1: minimun bound exceeded

    // Given a target servo angle, calculate the corresponding servo value
    servoPwmValue = InterpolationFloat(servo.calAngle1,servo.calAngle2,servo.calPwm1,
                                                     servo.calPwm2, servoAngle);

    // Check bounds
    if (servoPwmValue>=servo.UppPwmBound) {
        return 1;
    } 
    if (servoPwmValue<=servo.lwrPwmBound) {
        return -1;
    }

    // if bounds aren't exceeded return 0 or no error
    return 0;    
}