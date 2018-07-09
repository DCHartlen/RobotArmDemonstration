// Call after GcodeModule.h as this module uses the same serial communications 
// variables

// Contains functions for two modes, direct (controls servos) and cartesian

// initialization for gamepad (shared)
void setupGamepadControl();

// "update" function for direct gamepad controls
void DirectGamepadControl();

// "update" function for cartesian gamepad control
void CartesianGamepadControl();
