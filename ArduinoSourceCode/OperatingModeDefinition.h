// TODO: Add some header stuff to operating modes object
// What it does: Initializes an object which contains all operating states and 
// allows for transtion between them. 
// This style of changing modes heavily inspired by ThisArm firmware
// TODO: cite thisarm firmware here

// defines a specific type of function which takes no argument and lives inside
// the operating mode object
typedef void (*objectFunction) ();

// Define the prototype for each operating mode. Each operating mode has its own
// name, initialization, and update function. Update function runs every 10 ms
struct ModePrototype{
    String ModeName;
    objectFunction ModeInitialization;
    objectFunction ModeUpdate;
    objectFunction ModeSerialDebug;
    objectFunction LCDMessageUpdate;
};

void doNothingMode(){
  
}

#define nModes 7    //total number of operating modes

// Create object containing all operating modes
ModePrototype OperatingModes [nModes];  

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


