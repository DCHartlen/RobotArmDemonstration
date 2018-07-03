#define degreesOfFreedom 4

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
    int raw1;   // Raw servo at first calibration location
    int angle1; // Joint angle corresponding to first calibration location
    int raw2;   // Raw servo value at second calibration location
    int angle2; // Joint angle corresponding to second calibration location.
    float currentAngle;    // Current Joint angle
    int servoMin;    // Minimum raw value used for software limit
    int servoMax;   // Max safe raw value used for software limit
    int initialAngle;  // Initial angle of servo upon initialization
    Servo ServoObj;  // Servo object used for control and update.
};

// create an object containing a prototype for each servo
ServoObject ControlServos [degreesOfFreedom];

// Set a servo to a particular angle. Calls ServoBoundCheck to ensure that only
// valid angles are set. Returns an error code if bounds exceeded and sets servo
// max/min angle. 
int SetServoAngle(ServoObject servo, float proposedAngle);

// Given a servo and angle, assign raw servo value (pointer) and return error code
// code 1: angle exceeds maximum safe angle. code -1: servo exceed minimum bounds
int ServoBoundCheck(ServoObject servo, float servoAngle, int &rawServoValue);

void setupControlServos(){
    // During calibration, move arm to the angle1 and angle 2 positions. Read in
    // actual raw value and set in the appropriate area below. 

    // Base servo
    ControlServos[ServoBase].pin = 10;
    ControlServos[ServoBase].servoMin = 654;
    ControlServos[ServoBase].servoMax = 2400;
    ControlServos[ServoBase].raw1 = 1499;
    ControlServos[ServoBase].angle1 = 90; // Calibration loation: Straight forward
    ControlServos[ServoBase].raw2 = 2382;
    ControlServos[ServoBase].angle2 = 180; // Calibration location: arm faces 90deg left
    ControlServos[ServoBase].initialAngle = 90;
    ControlServos[ServoBase].ServoObj.attach(ControlServos[ServoBase].pin);
    ControlServos[ServoBase].ServoObj.write(ControlServos[ServoBase].initialAngle);
    ControlServos[ServoBase].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoBase].currentAngle = ControlServos[ServoBase].initialAngle;


    // Shoulder servo
    ControlServos[ServoShoulder].pin = 9;
    ControlServos[ServoShoulder].servoMin = 841;
    ControlServos[ServoShoulder].servoMax = 2248;
    ControlServos[ServoShoulder].raw1 = 1887;
    ControlServos[ServoShoulder].angle1 = 180;  // Calibration: shoulder parallel to ground
    ControlServos[ServoShoulder].raw2 = 865;
    ControlServos[ServoShoulder].angle2 = 90;   // Calibration: Shoulder vertical
    ControlServos[ServoShoulder].initialAngle = 180;
    ControlServos[ServoShoulder].ServoObj.attach(ControlServos[ServoShoulder].pin);
    ControlServos[ServoShoulder].ServoObj.write(ControlServos[ServoShoulder].initialAngle);
    ControlServos[ServoShoulder].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoShoulder].currentAngle = ControlServos[ServoShoulder].initialAngle;

        // Elbow servo
    ControlServos[ServoElbow].pin = 6;
    ControlServos[ServoElbow].servoMin = 600;
    ControlServos[ServoElbow].servoMax = 2400;
    ControlServos[ServoElbow].raw1 = 2400;
    ControlServos[ServoElbow].angle1 = 0;   // Calibration: Elbow crank parallel to ground
    ControlServos[ServoElbow].raw2 = 1292;
    ControlServos[ServoElbow].angle2 = 90;  // Calibration: Elbow crank vertical 
    ControlServos[ServoElbow].initialAngle = 90;
    ControlServos[ServoElbow].ServoObj.attach(ControlServos[ServoElbow].pin);
    ControlServos[ServoElbow].ServoObj.write(ControlServos[ServoElbow].initialAngle);
    ControlServos[ServoElbow].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoElbow].currentAngle = ControlServos[ServoElbow].initialAngle;

        // Claw servo
    ControlServos[ServoClaw].pin = 5;
    ControlServos[ServoClaw].servoMin = 800;
    ControlServos[ServoClaw].servoMax = 2400;
    ControlServos[ServoClaw].raw1 = 2400;
    ControlServos[ServoClaw].angle1 = 0;    // Calibration: Claw fully closed
    ControlServos[ServoClaw].raw2 = 1126;
    ControlServos[ServoClaw].angle2 = 90;   // Calibration: Claw open at right angle.
    ControlServos[ServoClaw].initialAngle = 45;
    ControlServos[ServoClaw].ServoObj.attach(ControlServos[ServoClaw].pin);
    ControlServos[ServoClaw].ServoObj.write(ControlServos[ServoClaw].initialAngle);
    ControlServos[ServoClaw].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoClaw].currentAngle = ControlServos[ServoClaw].initialAngle;
}