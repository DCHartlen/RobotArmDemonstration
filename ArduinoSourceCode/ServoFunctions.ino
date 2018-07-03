int SetServoAngle(ServoObject servo, float proposedAngle){
    // Set the servo's angle while respecting bounds
    int tempRaw = 0;    // Temporary servo raw. must respect bounds
    int errorCode;  // Error code returned by bound check
    //Check servo bounds, set temporary raw value, return error code
    errorCode = ServoBoundCheck(servo, proposedAngle,tempRaw);

    //Based on error code, write movement that servo will make
    switch(errorCode) {
        // No error code: set servo to proposed angle
        default:
            servo.currentRaw = tempRaw;
            servo.currentAngle = proposedAngle;
        // Error 1: max bound exceeded. Set to maximum angle
        case 1:
            servo.currentRaw = servo.servoMax;
            servo.currentAngle = InterpolationFloat(servo.angle1,servo.angle2,servo.raw1,
                                                     servo.raw2, servo.servoMax);
        // Error -1: minimum bound exceeded. Set to minimum angle.  
        case -1:
            servo.currentRaw = servo.servoMin;
            servo.currentAngle = InterpolationFloat(servo.angle1,servo.angle2,servo.raw1,
                                                     servo.raw2, servo.servoMin);
    }
    // Based on movement saved above, move the actual servo. 
    servo.ServoObj.write(servo.currentRaw);
    return errorCode;   //return the error code
}

int ServoBoundCheck(ServoObject servo, float servoAngle, int &rawServoValue){
    // Error codes: 1: maximum bound exceeded, -1: minimun bound exceeded

    // Given a target servo angle, calculate the corresponding servo value
    rawServoValue = InterpolationFloat(servo.angle1,servo.angle2,servo.raw1,
                                                     servo.raw2, servoAngle);

    // Check bounds
    if (rawServoValue>=servo.servoMax) {
        return 1;
    } 
    if (rawServoValue<=servo.servoMin) {
        return -1;
    }

    // if bounds aren't exceeded return 0 or no error
    return 0;    
}