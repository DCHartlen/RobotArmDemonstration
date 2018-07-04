void updateCalibrationMode(){
    // Update currently selected servo's raw value with encoder
    ControlServos[currentJointControlled].currentRaw = 
        ControlServos[currentJointControlled].currentRaw + controlEncoder.read();
    // Reset encoder tick counter to zero to prevent spiral out of control
    controlEncoder.write(0);

    // update all servos to currentRaw, not just the only one selected. Helps 
    // with stability, I think
    for(int iServo=0; iServo < degreesOfFreedom; iServo++) {
        ControlServos[iServo].ServoObj.write(ControlServos[iServo].currentRaw);
    }

}

void updateDirectEncoderControl(){
    // Use encoder to control the currently selected servo's angle. Encoder has
    // four ints per tick, so divide by for. each tick is a degree. 
    ControlServos[currentJointControlled].currentAngle = 
        ControlServos[currentJointControlled].currentAngle + controlEncoder.read()/4;
    // Reset encoder
    controlEncoder.write(0);

    // Update all servos
    for(int iServo=0; iServo < degreesOfFreedom; iServo ++) {
        ActuateServo(ControlServos[iServo], ControlServos[iServo].currentAngle);
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
    ControlServos[ServoClaw].currentAngle = clawAngle;
    ActuateServo(ControlServos[ServoClaw],ControlServos[ServoClaw].currentAngle);


}