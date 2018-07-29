// TODO: add header information
void SerialServoAngles(){
    // displays current servo, angle, and pulse width
    Serial.print("Servo: ");
    switch(currentJointControlled) {
        case 0: {
            Serial.print("Base    ");
            break;
        }
        case 1: {
            Serial.print("Shoulder");
            break;
        }
        case 2: {
            Serial.print("Elbow   ");
            break;
        }
        case 3: {
            Serial.print("Claw    ");
            break;
        }
    }
    Serial.print("  PWM: ");
    Serial.print(RobotServos[currentJointControlled].currentPwm);
    Serial.print("  Angle: ");
    Serial.println(RobotServos[currentJointControlled].currentAngle);
}

void SerialCartesian(){
    // displays current servo, angle, and pulse width
    Serial.print("DOF: ");
    switch(currentJointControlled) {
        case 0: {
            Serial.print("X   ");
            Serial.print(" Value: ");
            Serial.println(x);
            break;
        }
        case 1: {
            Serial.print("Y   ");
            Serial.print(" Value: ");
            Serial.println(y);
            break;
        }
        case 2: {
            Serial.print("Z   ");
            Serial.print(" Value: ");
            Serial.println(z);
            break;
        }
        case 3: {
            Serial.print("Claw");
            Serial.print(" Value: ");
            Serial.println(clawAngle);
            break;
        }
    }
}

void SerialJOintAngle(){
    // displays current servo, angle, and pulse width
    Serial.print("Joint: ");
    switch(currentJointControlled) {
        case 0: {
            Serial.print("Base    ");
            Serial.print(" Value: ");
            Serial.println(baseAngle);
            break;
        }
        case 1: {
            Serial.print("Shoulder");
            Serial.print(" Value: ");
            Serial.println(shoulderAngle);
            break;
        }
        case 2: {
            Serial.print("Elbow   ");
            Serial.print(" Value: ");
            Serial.println(elbowAngle);
            break;
        }
        case 3: {
            Serial.print("Claw    ");
            Serial.print(" Value: ");
            Serial.println(clawAngle);
            break;
        }
    }
}
