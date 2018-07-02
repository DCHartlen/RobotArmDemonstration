// Servos defined in an index object to allow easier enumeration

//create alias name of index number for readibility
#define ServoBase 0
#define ServoShoulder 1
#define ServoElbow 2
#define ServoClaw 3

// Create the prototype of a servo object
struct ServoObject {
    byte pin;   // corresponds to PWM pin arduino. MUST BE PWM
    int currentRaw;  // Current servo angle
    // int Idle;    //what does this do?
    int raw1;   // Raw servo at minimum angle
    int angle1; // Joint angle corresponding to first raw value
    int raw2;   // Raw servo value at emaximum angle
    int angle2; // Servo angle corresponding to max rotation
    float currentAngle;    // Current Joint angle
    int servoMin;    // Minimum raw value used for software limit
    int servoMax;   // Max safe raw value used for software limit
    int initialAngle;  // Initial angle of servo upon initialization
    Servo ServoObj;  // Servo object used for control and update.
}

// create an object containing a prototype for each servo
ServoObject ControlServos [degressOfFreedom];

void setupControlServos();