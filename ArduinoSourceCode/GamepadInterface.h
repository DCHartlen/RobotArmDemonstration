// Call after GcodeModule.h as this module uses the same serial communications 
// variables

// Contains functions for two modes, direct (controls servos) and cartesian

float updateHz = 1000/10;   // used to scale inputs to ##/s. Manually set 10

// initialization for direct control of servos with gamepad
void setupDirectGamepadControl();

// initialization for cartesian control of robot arm with gamepad
void setupCartesianGamepadControl();

// "update" function for direct gamepad controls
void DirectGamepadControl();

// "update" function for cartesian gamepad control
void CartesianGamepadControl();
