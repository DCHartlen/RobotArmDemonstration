// Given the simplicity of these functions, a detailed description is not
// provided for each one. Please refer to the documentation for your particular
// LCD screen if you require guideance. 

void LCDCalibration(){
    Screen.clear();
    Screen.setCursor(4,0);
    Screen.print("CALIBRATION");
    Screen.setCursor(0,1);
    Screen.print("BASE:");
    Screen.print(RobotServos[0].currentPwm);
    Screen.setCursor(10,1);
    Screen.print("SLDR:");
    Screen.print(RobotServos[1].currentPwm);
    Screen.setCursor(0,2);
    Screen.print("ELBW:");
    Screen.print(RobotServos[2].currentPwm);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(RobotServos[3].currentPwm);
}

void LCDDirectEncoder(){
    Screen.clear();
    Screen.setCursor(2,0);
    Screen.print("DIRECT ENCODER");
    Screen.setCursor(0,1);
    Screen.print("BASE:");
    Screen.print(RobotServos[0].currentAngle);
    Screen.setCursor(10,1);
    Screen.print("SLDR:");
    Screen.print(RobotServos[1].currentAngle);
    Screen.setCursor(0,2);
    Screen.print("ELBW:");
    Screen.print(RobotServos[2].currentAngle);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(RobotServos[3].currentAngle);
}

void LCDCartesianEncoder(){
    Screen.clear();
    Screen.setCursor(1,0);
    Screen.print("CARTESIAN ENCODER");
    Screen.setCursor(0,1);
    Screen.print("X:");
    Screen.print(x);
    Screen.setCursor(10,1);
    Screen.print("Y:");
    Screen.print(y);
    Screen.setCursor(0,2);
    Screen.print("Z:");
    Screen.print(z);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(clawAngle);
}

void LCDDirectGamepad(){
    Screen.clear();
    Screen.setCursor(2,0);
    Screen.print("DIRECT GAMEPAD");
    Screen.setCursor(0,1);
    Screen.print("BASE:");
    Screen.print(RobotServos[0].currentAngle);
    Screen.setCursor(10,1);
    Screen.print("SLDR:");
    Screen.print(RobotServos[1].currentAngle);
    Screen.setCursor(0,2);
    Screen.print("ELBW:");
    Screen.print(RobotServos[2].currentAngle);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(RobotServos[3].currentAngle);
}

void LCDCartesianGamepad(){
    Screen.clear();
    Screen.setCursor(1,0);
    Screen.print("CARTESIAN GAMEPAD");
    Screen.setCursor(0,1);
    Screen.print("X:");
    Screen.print(x);
    Screen.setCursor(10,1);
    Screen.print("Y:");
    Screen.print(y);
    Screen.setCursor(0,2);
    Screen.print("Z:");
    Screen.print(z);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(RobotServos[3].currentAngle);
}

void LCDGCodeControl(){
    Screen.clear();
    Screen.setCursor(3,0);
    Screen.print("GCODE CONTROL");
    Screen.setCursor(0,1);
    Screen.print("X:");
    Screen.print(x);
    Screen.setCursor(10,1);
    Screen.print("Y:");
    Screen.print(y);
    Screen.setCursor(0,2);
    Screen.print("Z:");
    Screen.print(z);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(RobotServos[3].currentAngle);
}

void LCDTicTacToeControl(){
    Screen.clear();
    Screen.setCursor(3,0);
    Screen.print("TIC TAC TOE");
    Screen.setCursor(0,1);
    Screen.print("X:");
    Screen.print(x);
    Screen.setCursor(10,1);
    Screen.print("Y:");
    Screen.print(y);
    Screen.setCursor(0,2);
    Screen.print("Z:");
    Screen.print(z);
    Screen.setCursor(10,2);
    Screen.print("CLAW:");
    Screen.print(RobotServos[3].currentAngle);
}
