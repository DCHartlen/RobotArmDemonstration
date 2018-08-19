// Given the simplicity of each debug message, a detailed description of each 
// is not provided. However, note that only the information associated with the
// currently selected joint/DOF is displayed, hence the switch-case statements.

void SerialCalibrationMode(){
    Serial.print("Calibration: Servo: ");
    switch(currentJointControlled) {
        case 0: {
            Serial.print("Base");
            break;
        }
        case 1: {
            Serial.print("Shoulder");
            break;
        }
        case 2: {
            Serial.print("Elbow");
            break;
        }
        case 3: {
            Serial.print("Claw");
            break;
        }
    }
    Serial.print("  PWM: ");
    Serial.print(RobotServos[currentJointControlled].currentPwm);
    Serial.print("  Angle: ");
    Serial.println(RobotServos[currentJointControlled].currentAngle);
}

void SerialDirectEncoder(){
    Serial.print("Direct Encoder: Servo: ");
    switch(currentJointControlled) {
        case 0: {
            Serial.print("Base");
            break;
        }
        case 1: {
            Serial.print("Shoulder");
            break;
        }
        case 2: {
            Serial.print("Elbow");
            break;
        }
        case 3: {
            Serial.print("Claw");
            break;
        }
    }
    Serial.print("  PWM: ");
    Serial.print(RobotServos[currentJointControlled].currentPwm);
    Serial.print("  Angle: ");
    Serial.println(RobotServos[currentJointControlled].currentAngle);
}

void SerialCartesianEncoder(){
    Serial.print("Cartesian Encoder: DOF: ");
    switch(currentJointControlled) {
        case 0: {
            Serial.print("X  ");
            Serial.print("Value: ");
            Serial.println(x);
            break;
        }
        case 1: {
            Serial.print("Y  ");
            Serial.print("Value: ");
            Serial.println(y);
            break;
        }
        case 2: {
            Serial.print("Z  ");
            Serial.print("Value: ");
            Serial.println(z);
            break;
        }
        case 3: {
            Serial.print("Claw  ");
            Serial.print("Value: ");
            Serial.println(clawAngle);
            break;
        }
    }
}
