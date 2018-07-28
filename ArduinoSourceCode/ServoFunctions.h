#define degreesOfFreedom 4

// Servos defined in an index object to allow easier enumeration

//create alias name of index number for readibility
#define ServoBase 0
#define ServoShoulder 1
#define ServoElbow 2
#define ServoClaw 3

// Create the prototype of a servo object
struct ServoObj {
    byte pin;           // corresponds to PWM pin arduino. MUST BE PWM
    int currentPwm;     // Current servo Pulse width (used for control)
    int calPwm1;        // Pulse width at calibration point 1
    int calAngle1;      // Physical angle at calibration point 1
    int calPwm2;        // Pulse width at calibration point 2
    int calAngle2;      // Physical angle at calibration point 2
    float currentAngle; // Current physical joint angle
    int lwrPwmBound;    // Minimum pwm value used for software limit
    int UppPwmBound;    // Max pwm value used for software limit
    int initialAngle;   // Initial angle of servo upon initialization
    Servo ServoControl; // Servo object used for control and update.
};

// create an object containing a prototype for each servo
ServoObj RobotServos [degreesOfFreedom];

// Set a servo to a particular angle. Calls ServoBoundCheck to ensure that only
// valid angles are set. Returns an error code if bounds exceeded and sets servo
// max/min angle. 
int ActuateServo(ServoObj servo, float proposedAngle);

// Given a servo and angle, assign raw servo value (pointer) and return error code
// code 1: angle exceeds maximum safe angle. code -1: servo exceed minimum bounds
int ServoBoundCheck(ServoObj servo, float servoAngle, int &rawServoValue);

void setupControlServos(){
    // During calibration, move arm to the calAngle1 and angle 2 positions. Read in
    // actual raw value and set in the appropriate area below. 

    // Base servo
    RobotServos[ServoBase].pin = 10;
    RobotServos[ServoBase].lwrPwmBound = 654;
    RobotServos[ServoBase].UppPwmBound = 2400;
    RobotServos[ServoBase].calPwm1 = 1499;
    RobotServos[ServoBase].calAngle1 = 90; // Calibration loation: Straight forward
    RobotServos[ServoBase].calPwm2 = 2382;
    RobotServos[ServoBase].calAngle2 = 180; // Calibration location: arm faces 90deg left
    RobotServos[ServoBase].initialAngle = 90;
    RobotServos[ServoBase].ServoControl.attach(RobotServos[ServoBase].pin);
    RobotServos[ServoBase].ServoControl.write(RobotServos[ServoBase].initialAngle);
    RobotServos[ServoBase].currentPwm = 
        RobotServos[ServoBase].ServoControl.readMicroseconds();
    RobotServos[ServoBase].currentAngle = RobotServos[ServoBase].initialAngle;


    // Shoulder servo
    RobotServos[ServoShoulder].pin = 9;
    RobotServos[ServoShoulder].lwrPwmBound = 841;
    RobotServos[ServoShoulder].UppPwmBound = 2248;
    RobotServos[ServoShoulder].calPwm1 = 1887;
    RobotServos[ServoShoulder].calAngle1 = 180;  // Calibration: shoulder parallel to ground
    RobotServos[ServoShoulder].calPwm2 = 865;
    RobotServos[ServoShoulder].calAngle2 = 90;   // Calibration: Shoulder vertical
    RobotServos[ServoShoulder].initialAngle = 180;
    RobotServos[ServoShoulder].ServoControl.attach(RobotServos[ServoShoulder].pin);
    RobotServos[ServoShoulder].ServoControl.write(RobotServos[ServoShoulder].initialAngle);
    RobotServos[ServoShoulder].currentPwm = 
        RobotServos[ServoShoulder].ServoControl.readMicroseconds(); 
    RobotServos[ServoShoulder].currentAngle = RobotServos[ServoShoulder].initialAngle;

        // Elbow servo
    RobotServos[ServoElbow].pin = 6;
    RobotServos[ServoElbow].lwrPwmBound = 600;
    RobotServos[ServoElbow].UppPwmBound = 2400;
    RobotServos[ServoElbow].calPwm1 = 2400;
    RobotServos[ServoElbow].calAngle1 = 0;   // Calibration: Elbow crank parallel to ground
    RobotServos[ServoElbow].calPwm2 = 1292;
    RobotServos[ServoElbow].calAngle2 = 90;  // Calibration: Elbow crank vertical 
    RobotServos[ServoElbow].initialAngle = 90;
    RobotServos[ServoElbow].ServoControl.attach(RobotServos[ServoElbow].pin);
    RobotServos[ServoElbow].ServoControl.write(RobotServos[ServoElbow].initialAngle);
    RobotServos[ServoElbow].currentPwm = 
        RobotServos[ServoElbow].ServoControl.readMicroseconds();
    RobotServos[ServoElbow].currentAngle = RobotServos[ServoElbow].initialAngle;

        // Claw servo
    RobotServos[ServoClaw].pin = 5;
    RobotServos[ServoClaw].lwrPwmBound = 800;
    RobotServos[ServoClaw].UppPwmBound = 2400;
    RobotServos[ServoClaw].calPwm1 = 2400;
    RobotServos[ServoClaw].calAngle1 = 0;    // Calibration: Claw fully closed
    RobotServos[ServoClaw].calPwm2 = 1126;
    RobotServos[ServoClaw].calAngle2 = 90;   // Calibration: Claw open at right angle.
    RobotServos[ServoClaw].initialAngle = 45;
    RobotServos[ServoClaw].ServoControl.attach(RobotServos[ServoClaw].pin);
    RobotServos[ServoClaw].ServoControl.write(RobotServos[ServoClaw].initialAngle);
    RobotServos[ServoClaw].currentPwm = 
        RobotServos[ServoClaw].ServoControl.readMicroseconds();
    RobotServos[ServoClaw].currentAngle = RobotServos[ServoClaw].initialAngle;
}