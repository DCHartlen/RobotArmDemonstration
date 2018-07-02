// TODO Add some header stuff to operating modes object
// What it does: Initializes an object which contains all operating states and 
// allows for transtion between them. 
// This style of changing modes heavily inspired by ThisArm firmware
// TODO cite thisarm firmware here

// defines a specific type of function which takes no argument and lives inside
// the operating mode object
typedef void(*objectFunction) ();

// Define the prototype for each operating mode. Each operating mode has its own
// name, initialization, and update function. Update function runs every 10 ms
struct ModePrototype{
    char *ModeName;
    objectFunction ModeInitialization;
    ObjectFromLresult ModeUpdate;
};

#define nModes 1    //total number of operating modes

// Create object containing all operating modes
ModePrototype OperatingModes [nModes];  

// Create initialization method which populates the object of operating modes
void setupOperatingModes();

