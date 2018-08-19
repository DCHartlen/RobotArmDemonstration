// Given the simplicity of these functions, a detailed description is not
// provided for each one. Please refer to the documentation for your particular
// LCD screen if you require guideance. 

void LCDCalibration(){
    Screen.setCursor(0,4);
    Screen.print("CALIBRATION");
    Screen.setCursor(1,0);
    Screen.print("BASE: ");
    Screen.print(RobotServos[0].currentAngle);
    Screen.setCursor(1,10);
    Screen.print("SLDR: ");
    Screen.print(RobotServos[1].currentAngle);
    Screen.setCursor(2,0);
    Screen.print("ELBW: ");
    Screen.print(RobotServos[2].currentAngle);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(RobotServos[3].currentAngle);
}

void LCDDirectEncoder(){
    Screen.setCursor(0,2);
    Screen.print("DIRECT ENCODER");
    Screen.setCursor(1,0);
    Screen.print("BASE: ");
    Screen.print(baseAngle);
    Screen.setCursor(1,10);
    Screen.print("SLDR: ");
    Screen.print(shoulderAngle);
    Screen.setCursor(2,0);
    Screen.print("ELBW: ");
    Screen.print(elbowAngle);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(clawAngle);
}

void LCDCartesianEncoder(){
    Screen.setCursor(0,1);
    Screen.print("CARTESIAN ENCODER");
    Screen.setCursor(1,0);
    Screen.print("X: ");
    Screen.print(x);
    Screen.setCursor(1,10);
    Screen.print("Y: ");
    Screen.print(y);
    Screen.setCursor(2,0);
    Screen.print("Z: ");
    Screen.print(z);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(clawAngle);
}

void LCDDirectGamepad(){
    Screen.setCursor(0,2);
    Screen.print("DIRECT GAMEPAD");
    Screen.setCursor(1,0);
    Screen.print("BASE: ");
    Screen.print(baseAngle);
    Screen.setCursor(1,10);
    Screen.print("SLDR: ");
    Screen.print(shoulderAngle);
    Screen.setCursor(2,0);
    Screen.print("ELBW: ");
    Screen.print(elbowAngle);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(clawAngle);
}

void LCDCartesianGamepad(){
    Screen.setCursor(0,1);
    Screen.print("CARTESIAN GAMEPAD");
    Screen.setCursor(1,0);
    Screen.print("X: ");
    Screen.print(x);
    Screen.setCursor(1,10);
    Screen.print("Y: ");
    Screen.print(y);
    Screen.setCursor(2,0);
    Screen.print("Z: ");
    Screen.print(z);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(clawAngle);
}

void LCDGCodeControl(){
    Screen.setCursor(0,3);
    Screen.print("GCODE CONTROL");
    Screen.setCursor(1,0);
    Screen.print("X: ");
    Screen.print(x);
    Screen.setCursor(1,10);
    Screen.print("Y: ");
    Screen.print(y);
    Screen.setCursor(2,0);
    Screen.print("Z: ");
    Screen.print(z);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(clawAngle);
}

void LCDTicTacToeControl(){
    Screen.setCursor(0,3);
    Screen.print("TIC TAC TOE");
    Screen.setCursor(1,0);
    Screen.print("X: ");
    Screen.print(x);
    Screen.setCursor(1,10);
    Screen.print("Y: ");
    Screen.print(y);
    Screen.setCursor(2,0);
    Screen.print("Z: ");
    Screen.print(z);
    Screen.setCursor(2,10);
    Screen.print("CLAW: ");
    Screen.print(clawAngle);
}
