// Operating function used for calibration. Encoder directly controls the PWM
// value of the currently selected servo. In this mode, the PWM bounds of each
// servo are not respected, so extreme care must be taken not to damage the
// robotic arm. This mode is not designed for regular operation. Use 
// "updateDirectEncoderControl()" instead. 
void updateCalibrationMode(){
    // Update currently selected servo's raw value with encoder
    RobotServos[currentJointControlled].currentPwm += controlEncoder.read()*2;
    // Reset encoder tick counter to zero to prevent spiral out of control
    controlEncoder.write(0);

    // update all servos to currentPwm, not just the only one selected. Helps 
    // with stability, I think
    for(int iServo=0; iServo < degreesOfFreedom; iServo++) {
        RobotServos[iServo].ServoControl.writeMicroseconds(RobotServos[iServo].currentPwm);
    }

}

// Operating function used for direct control of joint angle using an encoder. 
// Servo bounds are respected by this function. 
void updateDirectEncoderControl(){
    // Use encoder to control the currently selected servo's angle. Encoder has
    // four ints per tick, so divide by for. each tick is a degree. 
    RobotServos[currentJointControlled].currentAngle += float(controlEncoder.read())/4.0;
    // Reset encoder
    controlEncoder.write(0);

    // Update all servos
    for(int iServo=0; iServo < degreesOfFreedom; iServo ++) {
        ActuateServo(RobotServos[iServo], RobotServos[iServo].currentAngle);
    }
};

// Operating function used cartesian control of the end effector's location
// using the encoder. While servo bounds are respected by this function, no
// limits are placed on the coordinates a robot can reach. As such, x,y,z value 
// can be increased or decreased indefinately even though the arm will not
// respond. 
void updateCartesianEncoderControl() {
    // Based on "currentJointControlled", choose DoF to increment
    switch(currentJointControlled){
        case 0:     // x direction control
            x += controlEncoder.read();
            break;
        case 1:     // y direction control
            y += controlEncoder.read();
            break;
        case 2:     // z direction control
            z += controlEncoder.read();
            break;
        case 3:     // Claw control
            clawAngle += controlEncoder.read();
            break;
    }
    // reset encoder to prevent runaway
    controlEncoder.write(0);

    // Move all joints using inverse kinematics
    MoveIK(x,y,z,shoulderAngle,elbowAngle,baseAngle);

    // Actuate the claw
    RobotServos[ServoClaw].currentAngle = clawAngle;
    ActuateServo(RobotServos[ServoClaw], RobotServos[ServoClaw].currentAngle);
}