//  GamepadInterface
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//
//  Gamepad control requires an computer communicating with via UART with the
//  Arduino. Processing of Xbox controller inputs is handled by the computer. 
//  See "GampadControl.py" under "PythonSourceCode" Xbox controller interface.
//  
//  This code parses input commands and commands the servos to move. All input
//  commands are acknowledged with the computer. Both direct and cartesian move
//  commands are handled by this code. Note that all UART send and receive
//  operations are blocking.

float updateHz = 1000/25;   // used to scale inputs to units/s. Manually set 10

// initialization for direct control of servos with gamepad
void setupDirectGamepadControl();

// initialization for cartesian control of robot arm with gamepad
void setupCartesianGamepadControl();

// "update" function for direct gamepad controls
void DirectGamepadControl();

// "update" function for cartesian gamepad control
void CartesianGamepadControl();
