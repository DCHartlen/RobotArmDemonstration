//  ServoFunctions - Bounds, Calibration, and Control of all servos
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//
//  This source code controls all servos. This includes mapping of internal servo
//  angles to useable joint angles, the working bounds of the joints, and
//  actuation of all servos taking mapping and bounds into account. 
// 
//  Note: Calibration assumes linear mapping between servo PWM value and joint
//  angle. While valid for robot arms which are based on "palletizing" designs,
//  these assumptions are not valid for all robotic arms and must be adjusted 
//  for your particular arm. "Palletizing"-type robots include the MeArm, the 
//  three iterations of the EEZYBotArm, and similar robots.
//
//  To perform calibration, it is easiest to disassembly the robot and
//  individually move each joint to the angle listed in "CalAngle#". Record the 
//  servo PWM value at teach angle (readMicroseconds()) and insert these values
//  to "calPwm#". Must be in the Calibration operating mode. 

#define degreesOfFreedom 4

//create alias name of index number for readibility
#define ServoBase 0
#define ServoShoulder 1
#define ServoElbow 2
#define ServoClaw 3

// Create the prototype of a servo control object. This object allows limits,
// calibration, and control associated with a servo to be stored in one object. 
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

// create an object containing a prototype for each servo. RobotServos is used
// for all control of servos during operation. 
ServoObj RobotServos [degreesOfFreedom];

// Function actuates a particular servo to a proposed angle. If proposed angle 
// is outside the operating bounds of the robotic arm, the arm will move to the
// operating bounds of the robot instead. Robotic arm will never exceed the 
// operating bounds defined in "setupControlServos"
//
// Example usage:
//  ActuateServo(RobotServos[ServoElbow], 90.2)
int ActuateServo(ServoObj &servo, float proposedAngle);

// Assigns bounds and calibration values to all servos. This function also
// attaches a servo to a particular pin on the arduino. Note: PWM pins are not
// required for the Arduino-supplied "Servo.h" library, so literally any digital
// or analog pin could be used. 
void setupControlServos(){
    // During calibration, move arm to the calAngle1 and angle 2 positions. Read in
    // actual raw value and set in the appropriate area below. 

    // Base servo
    RobotServos[ServoBase].pin = 8;
    RobotServos[ServoBase].lwrPwmBound = 685;
    RobotServos[ServoBase].UppPwmBound = 2375;
    RobotServos[ServoBase].calPwm1 = 1692;
    RobotServos[ServoBase].calAngle1 = 90; // Calibration loation: Straight forward
    RobotServos[ServoBase].calPwm2 = 836;
    RobotServos[ServoBase].calAngle2 = 45; // Calibration location: arm faces 90deg Right
    RobotServos[ServoBase].initialAngle = 90;
    RobotServos[ServoBase].ServoControl.attach(RobotServos[ServoBase].pin);
    RobotServos[ServoBase].ServoControl.write(RobotServos[ServoBase].initialAngle);
    RobotServos[ServoBase].currentPwm = 
    RobotServos[ServoBase].ServoControl.readMicroseconds();
    RobotServos[ServoBase].currentAngle = RobotServos[ServoBase].initialAngle;


    // Shoulder servo
    RobotServos[ServoShoulder].pin = 7;
    RobotServos[ServoShoulder].lwrPwmBound = 812;
    RobotServos[ServoShoulder].UppPwmBound = 2364;
    RobotServos[ServoShoulder].calPwm1 = 852;
    RobotServos[ServoShoulder].calAngle1 = 0;  // Calibration: shoulder parallel to ground
    RobotServos[ServoShoulder].calPwm2 = 1676;
    RobotServos[ServoShoulder].calAngle2 = 90;   // Calibration: Shoulder vertical
    RobotServos[ServoShoulder].initialAngle = 90;
    RobotServos[ServoShoulder].ServoControl.attach(RobotServos[ServoShoulder].pin);
    RobotServos[ServoShoulder].ServoControl.write(RobotServos[ServoShoulder].initialAngle);
    RobotServos[ServoShoulder].currentPwm = 
    RobotServos[ServoShoulder].ServoControl.readMicroseconds(); 
    RobotServos[ServoShoulder].currentAngle = RobotServos[ServoShoulder].initialAngle;

        // Elbow servo
    RobotServos[ServoElbow].pin = 6;
    RobotServos[ServoElbow].lwrPwmBound = 680;
    RobotServos[ServoElbow].UppPwmBound = 2120;
    RobotServos[ServoElbow].calPwm1 = 1892;
    RobotServos[ServoElbow].calAngle1 = 0;   // Calibration: Elbow crank parallel to ground
    RobotServos[ServoElbow].calPwm2 = 1020;
    RobotServos[ServoElbow].calAngle2 = 90;  // Calibration: Elbow crank vertical 
    RobotServos[ServoElbow].initialAngle = 75;
    RobotServos[ServoElbow].ServoControl.attach(RobotServos[ServoElbow].pin);
    RobotServos[ServoElbow].ServoControl.write(RobotServos[ServoElbow].initialAngle);
    RobotServos[ServoElbow].currentPwm = 
    RobotServos[ServoElbow].ServoControl.readMicroseconds();
    RobotServos[ServoElbow].currentAngle = RobotServos[ServoElbow].initialAngle;

        // Claw servo
    RobotServos[ServoClaw].pin = 5;
    RobotServos[ServoClaw].lwrPwmBound = 770;
    RobotServos[ServoClaw].UppPwmBound = 2200;
    RobotServos[ServoClaw].calPwm1 = 780;
    RobotServos[ServoClaw].calAngle1 = 0;    // Calibration: Claw fully closed
    RobotServos[ServoClaw].calPwm2 = 1452;
    RobotServos[ServoClaw].calAngle2 = 90;   // Calibration: Claw open at right angle.
    RobotServos[ServoClaw].initialAngle = 45;
    RobotServos[ServoClaw].ServoControl.attach(RobotServos[ServoClaw].pin);
    RobotServos[ServoClaw].ServoControl.write(RobotServos[ServoClaw].initialAngle);
    RobotServos[ServoClaw].currentPwm = 
    RobotServos[ServoClaw].ServoControl.readMicroseconds();
    RobotServos[ServoClaw].currentAngle = RobotServos[ServoClaw].initialAngle;
}