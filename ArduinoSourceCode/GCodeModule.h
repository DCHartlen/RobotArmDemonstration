#define MAX_BUF              (32)  // What is the longest message Arduino can store?
#define MAX_FEEDRATE         (1000000) //millimeters per minute
#define MIN_FEEDRATE         (1)      //millimeters per minute

char flagAbsolute=1;  // commands in absolute coordinates
char serialBuffer[bufferLength];
int currentBufferLength;
float feedRate

void GCodeControl();

void printCurrentLocation();