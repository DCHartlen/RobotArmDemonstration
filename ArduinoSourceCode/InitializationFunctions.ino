void setupCalibrationMode(){
    Serial.println("Calibration Mode Entered");
    // set the currentPwm value of all encoder to 1500, which is approximately
    // the middle of thier stroke. Its a safe move nonetheless
    for (int iServo = 0; iServo<degreesOfFreedom; iServo++) {
        RobotServos[iServo].currentPwm = 1500;
        RobotServos[iServo].ServoControl.writeMicroseconds(RobotServos[iServo].currentPwm);
    }
}

void setupDirectEncoderControl() {
    Serial.println("Direct Encoder Control");
    // Set all servos to thier initial angles. Requires calibrated servos.
    float temp;
    for (int iServo = 0; iServo<degreesOfFreedom; iServo++) {
        temp = RobotServos[iServo].initialAngle;
        ActuateServo(RobotServos[iServo], temp);
    }
}

void setupCartesianEncoderControl() {
    Serial.println("Cartesian Encoder Control");
    clawAngle = 45;
    x = 0;
    y = 80;
    z = 25;
}