#define bufferLength (32)  // What is the longest message Arduino can store?

char flagAbsolute=1;  // commands in absolute coordinates
char serialBuffer[bufferLength];
int currentBufferLength;
float feedRate = 1000;

// initialization function for gcode control
void setupGCodeControl();
// initialization for tic tac toe
void setupTicTacToeControl();
// "update" function for gcode control
void GCodeControl();
// print the current location of the manipulator
void printCurrentLocation();