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

// typedef void (*VoidProcedure) ();


void EmptyProc (){
  
}
// struct StageContainer {
//   char *StageName;
//   VoidProcedure InitProc;
//   VoidProcedure ControlProc ;
//   VoidProcedure DisplayProc;
//   VoidProcedure CickProc ;
// };

#define nModes 1    //total number of operating modes

// Create object containing all operating modes
ModePrototype OperatingModes [nModes];  
// StageContainer OperatingModes [nModes];

// Create initialization method which populates the object of operating modes
// void setupOperatingModes() {
//     // Mode 1 (0): directly control servo angle using encoder.
//     OperatingModes[0].ModeName = "DirectEncoderControl";
//     OperatingModes[0].ModeInitialization = DefaultInit
//     OperatingModes[0].ModeUpdate = updateDirectEncoderControl;

//     // TODO Add more functions
// }

void setupOperatingModes() {
    // Mode 1 (0): directly control servo angle using encoder.
    OperatingModes[0].ModeName = "DirectEncoderControl";
    OperatingModes[0].ModeInitialization = setupDirectEncoderControl;
    OperatingModes[0].ModeUpdate = setupDirectEncoderControl;

    // TODO Add more functions
}


