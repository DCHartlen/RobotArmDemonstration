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
        SetServoAngle(ControlServos[iServo], ControlServos[iServo].currentAngle);
    }
};