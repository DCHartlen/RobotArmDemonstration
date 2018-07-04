void setupCalibrationMode(){
    // set the currentRaw value of all encoder to 1500, which is approximately
    // the middle of thier stroke. Its a safe move nonetheless
    for (int iServo = 0; iServo<degreesOfFreedom; iServo++) {
        ControlServos[iServo].currentRaw = 1500;
        ControlServos[iServo].ServoObj.write(ControlServos[iServo].currentRaw);
    }
}

void setupDirectEncoderControl() {
    // Set all servos to thier initial angles. Requires calibrated servos.
    float temp;
    for (int iServo = 0; iServo<degreesOfFreedom; iServo++) {
        temp = ControlServos[iServo].initialAngle;
        SetServoAngle(ControlServos[iServo], temp);
    }
}