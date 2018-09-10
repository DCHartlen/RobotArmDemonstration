//  GCodeModule - Processes G-code commands
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//
//  G0-code control requires a computer to act as a g-code sender. This source
//  code does not generate source code, only process commands and move the 
//  arm. Rudimentary g-code sending is handled by "SerialInterface.py" under
//  "PythonSourceCode"
//
//  This source code handles multiple code commands, which are listed below. A
//  list of commands can also be access by sending "M100" to the arduino. This 
//  module is adapted from the "ThisArm" firmware developed by "Iamjonah".
// 
//  Available g-code commands:
//    G00 [X/Y/Z(mm)]; - move to location as fast as possible
//    G01 [X/Y/Z(mm)] [F(mm/s)]; - move at speed "F" to location.
//    G04 P[seconds]; - Pause movement
//    G90; - Robot operates in absolute coordinates
//    G91; - Robot operates in coordinate system relative to current coordinates
//    M100; - Print help messsage
//    M106 [S(Claw angle)]; - Actuate end effector
//    M114; - print current position and feedrate

#define bufferLength (32)  // What is the longest message Arduino can store?

char flagAbsolute=1;  // commands default to absolute coordinates
char serialBuffer[bufferLength];
int currentBufferLength;
float feedRate = 5000;  // default movement speed

// initialization function for gcode control
void setupGCodeControl();
// initialization for tic tac toe
void setupTicTacToeControl();
// "update" function for gcode control
void GCodeControl();
// print the current location of the manipulator
void printCurrentLocation();