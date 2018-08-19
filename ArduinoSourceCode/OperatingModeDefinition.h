//  OperatingModeDefinition - Defines behavior of each operating mode
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//  
//  This source code defines the behavior of each operating mode. This is 
//  accomplished by placing each operating mode in one large structure,
//  including each modes initialzation, update, debug and display functions.
//  This approach allows for enumeration of each operating mode, making the
//  switch between modes easy. Otherwise, firmware for each mode would need to 
//  be flashed to change opeartion. 
//
//  This methodolgy was adapted from the "ThisArm" firmware developed by
//  "Iamjonah".

// This typedef allows for the structure to take a function name as a value and
// for that function to be called by the structure. 
typedef void (*objectFunction) ();

// Define the prototype for each operating mode. Each operating mode has its own
// name, initialization, and update function.
struct ModePrototype{
    String ModeName;
    objectFunction ModeInitialization;
    objectFunction ModeUpdate;
    objectFunction ModeSerialDebug;
    objectFunction LCDMessageUpdate;
};

// Define an empty function if no function is needed. For example, this is used
// with g-code control as serial communications are handled by internal functions. 
void doNothingMode(){ 
}

#define nModes 7    //total number of operating modes

// Create structure containing all operating modes
ModePrototype OperatingModes [nModes];  

// Define all operating functions. Each operating function must have, at
// minimum, a name, an initialization function, and an update/operating
// function. Seral debug and LCD messages are not required, but must be assigned
// "doNothingMode" if not used. 
void setupOperatingModes() {
    // Mode 1 (0); Calibration mode, Encoder directly controls servo raw value, 
    // No logic used for max and min. 
    OperatingModes[0].ModeName = "CalibrationMode";
    OperatingModes[0].ModeInitialization = setupCalibrationMode;
    OperatingModes[0].ModeUpdate = updateCalibrationMode;
    OperatingModes[0].ModeSerialDebug = SerialCalibrationMode;
    OperatingModes[0].LCDMessageUpdate = LCDCalibration;

    // Mode 2 (1): Directly control the angle of the servo using the encoder
    OperatingModes[1].ModeName = "DirectEncoderControl";
    OperatingModes[1].ModeInitialization = setupDirectEncoderControl;
    OperatingModes[1].ModeUpdate = updateDirectEncoderControl;
    OperatingModes[1].ModeSerialDebug = SerialDirectEncoder;
    OperatingModes[1].LCDMessageUpdate = LCDDirectEncoder;

    // Mode 3 (2): Directly control manipulator position using the encoder
    OperatingModes[2].ModeName = "CartesianEncoderControl";
    OperatingModes[2].ModeInitialization = setupCartesianEncoderControl;
    OperatingModes[2].ModeUpdate = updateCartesianEncoderControl;
    OperatingModes[2].ModeSerialDebug = SerialCartesianEncoder;
    OperatingModes[2].LCDMessageUpdate = LCDCartesianEncoder;

    // Mode 4 (3): G-Code control
    OperatingModes[3].ModeName = "GCodeControl";
    OperatingModes[3].ModeInitialization = setupGCodeControl; // found in GCodeModule
    OperatingModes[3].ModeUpdate = GCodeControl;    // found in GCodeControl
    OperatingModes[3].ModeSerialDebug = doNothingMode;
    OperatingModes[3].LCDMessageUpdate = LCDGCodeControl;

    // Mode 5 (4): Gamepad Control of joints
    OperatingModes[4].ModeName = "DirectGamepadControl";
    OperatingModes[4].ModeInitialization = setupDirectGamepadControl; 
    OperatingModes[4].ModeUpdate = DirectGamepadControl;
    OperatingModes[4].ModeSerialDebug = doNothingMode;
    OperatingModes[4].LCDMessageUpdate = LCDDirectGamepad;

    // Mode 6 (5): Gamepad Control of location
    OperatingModes[5].ModeName = "CartesianGamepadControl";
    OperatingModes[5].ModeInitialization = setupCartesianGamepadControl; 
    OperatingModes[5].ModeUpdate = CartesianGamepadControl;
    OperatingModes[5].ModeSerialDebug = doNothingMode;
    OperatingModes[5].LCDMessageUpdate = LCDCartesianGamepad;

    // Mode 7 (6): Mode for Tic-Tac-Toe (which is controlled using gcode)
    OperatingModes[6].ModeName = "TicTacToeMode";
    OperatingModes[6].ModeInitialization = setupTicTacToeControl; // found in GCodeModule
    OperatingModes[6].ModeUpdate = GCodeControl;    // found in GCodeControl
    OperatingModes[6].ModeSerialDebug = doNothingMode;
    OperatingModes[6].LCDMessageUpdate = LCDTicTacToeControl;
}


