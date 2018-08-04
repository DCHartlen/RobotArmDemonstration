void updateCalibrationMode(){
    // Update currently selected servo's raw value with encoder
    RobotServos[currentJointControlled].currentPwm += controlEncoder.read()*4;
    // Reset encoder tick counter to zero to prevent spiral out of control
    controlEncoder.write(0);

    // update all servos to currentPwm, not just the only one selected. Helps 
    // with stability, I think
    for(int iServo=0; iServo < degreesOfFreedom; iServo++) {
        RobotServos[iServo].ServoControl.writeMicroseconds(RobotServos[iServo].currentPwm);
    }

}

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

void updateCartesianEncoderControl() {
    // control x,y,z with encoder.

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