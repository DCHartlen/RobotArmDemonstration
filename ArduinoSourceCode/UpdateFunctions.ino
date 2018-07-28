void updateCalibrationMode(){
    // Update currently selected servo's raw value with encoder
    RobotServos[currentJointControlled].currentPwm = 
        RobotServos[currentJointControlled].currentPwm + controlEncoder.read()*2;
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
    RobotServos[currentJointControlled].currentAngle = 
        RobotServos[currentJointControlled].currentAngle + controlEncoder.read()/4;
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
            x = x + controlEncoder.read();
            controlEncoder.write(0);
            break;
        case 1:     // y direction control
            y = y + controlEncoder.read();
            controlEncoder.write(0);
            break;
        case 2:     // z direction control
            z = z + controlEncoder.read();
            controlEncoder.write(0);
            break;
        case 3:     // Claw control
            clawAngle = clawAngle + controlEncoder.read();
            controlEncoder.write(0);
            break;
    }

    // Move all joints using inverse kinematics
    MoveIK(x,y,z,shoulderAngle,elbowAngle,baseAngle);

    // Actuate the claw
    RobotServos[ServoClaw].currentAngle = clawAngle;
    ActuateServo(RobotServos[ServoClaw], RobotServos[ServoClaw].currentAngle);


}