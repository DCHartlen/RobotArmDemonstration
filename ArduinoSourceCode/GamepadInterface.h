// Call after GcodeModule.h as this module uses the same serial communications 
// variables

// Contains functions for two modes, direct (controls servos) and cartesian

float updateHz = 1000/10;   // used to scale inputs to ##/s. Manually set 10

// initialization for gamepad (shared)
void setupGamepadControl();

// "update" function for direct gamepad controls
void DirectGamepadControl();

// "update" function for cartesian gamepad control
void CartesianGamepadControl();
