// Initialization function for calibration operating mode. Sets the PWM value
// for all servos to 1500 using direct servo control.
void setupCalibrationMode(){
    Serial.println("Calibration Mode Entered");
    // set the currentPwm value of all encoder to 1500, which is approximately
    // the middle of thier stroke. Its a safe move nonetheless
    for (int iServo = 0; iServo<degreesOfFreedom; iServo++) {
        RobotServos[iServo].currentPwm = 1500;
        RobotServos[iServo].ServoControl.writeMicroseconds(RobotServos[iServo].currentPwm);
    }
}

// Initialization for direct control of robot joints using encoder. Joint bounds
// are respected in this mode. All servos are set to initial angle specified in
// the servo structure.
void setupDirectEncoderControl() {
    Serial.println("Direct Encoder Control");
    // Set all servos to thier initial angles. Requires calibrated servos.
    float temp;
    for (int iServo = 0; iServo<degreesOfFreedom; iServo++) {
        temp = RobotServos[iServo].initialAngle;
        ActuateServo(RobotServos[iServo], temp);
    }
}

// Initialization for cartesian control of arm using the encoder. Joint bounds
// are respected, but work envelope bounds are not. Move end effector to home 
// position and opens end effector half way.
void setupCartesianEncoderControl() {
    Serial.println("Cartesian Encoder Control");
    clawAngle = 45;
    x = 0;
    y = 80;
    z = 80;
    // LinearMove(x,y,z, 1000);
    BasicMove(x,y,z);
    ActuateServo(RobotServos[4], clawAngle);
}