void updateCalibrationMode(){
    // Update currently selected servo with encoder
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
    // Use encoder to control the currently selected servo. Update all servos
};