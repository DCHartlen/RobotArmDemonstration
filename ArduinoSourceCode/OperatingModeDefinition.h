// TODO Add some header stuff to operating modes object
// What it does: Initializes an object which contains all operating states and 
// allows for transtion between them. 
// This style of changing modes heavily inspired by ThisArm firmware
// TODO cite thisarm firmware here

// defines a specific type of function which takes no argument and lives inside
// the operating mode object
typedef void (*objectFunction) ();

// Define the prototype for each operating mode. Each operating mode has its own
// name, initialization, and update function. Update function runs every 10 ms
struct ModePrototype{
    char *ModeName;
    objectFunction ModeInitialization;
    objectFunction ModeUpdate;
};

void doNothingMode (){
  
}

#define nModes 4    //total number of operating modes

// Create object containing all operating modes
ModePrototype OperatingModes [nModes];  

void setupOperatingModes() {
    // Mode 1 (0); Calibration mode, Encoder directly controls servo raw value, 
    // No logic used for max and min. 
    OperatingModes[1].ModeName = "CalibrationMode";
    OperatingModes[1].ModeInitialization = setupCalibrationMode;
    OperatingModes[1].ModeUpdate = updateCalibrationMode;

    // Mode 2 (1): Directly control the angle of the servo using the encoder
    OperatingModes[1].ModeName = "DirectEncoderControl";
    OperatingModes[1].ModeInitialization = setupDirectEncoderControl;
    OperatingModes[1].ModeUpdate = updateDirectEncoderControl;

    // Mode 3 (2): Directly control the angle of the servo using the encoder
    OperatingModes[2].ModeName = "CartesianEncoderControl";
    OperatingModes[2].ModeInitialization = setupCartesianEncoderControl;
    OperatingModes[2].ModeUpdate = updateCartesianEncoderControl;

    // Mode 4 (3): Directly control the angle of the servo using the encoder
    OperatingModes[3].ModeName = "GCodeControl";
    OperatingModes[3].ModeInitialization = setupGCodeControl; // found in GCodeModule
    OperatingModes[3].ModeUpdate = GCodeControl;    // found in GCodeControl
}


