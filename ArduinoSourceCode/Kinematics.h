//  Kinematics - Performs inverse kinematis and moves the arm.
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//
//  Robot arm must be 3+1 degrees of freedom. Further, arm must have only one
//  DOF controlling azimuth, with the radial component of motion being 
//  controlled by two rotational joints. 
//
//  The closed form IK solution presented here is taken from the "ThisArm" 
//  firmware developed by "Iamjonah", although movement commands have been 
//  developed independantly. 
//
//  Robot geometry must defined in this function.
//
//  Note that this header does not have an associated *.ino file. I know this is
//  poor practice, but it is undeniably effective. 

//Robot Geometry
float lowerArmLength =  134.5;
float upperArmLength =  148.0;
// Assumes that operating envelope is a revolved rectangle. Not entirely true,
// but good enough. Future work should develop a more robust IK solution. 
float minEnvelopRadius =  15.0;
float maxEnvelopRadius =  115.0;

float ShoulderOffset = 10.0;     // Distance which shoulder is offset from radial center.

// Function performs all math required to determine angle of shoulder and elbow
// servos to reach a specific 2D location. This 2D location is defined radially
// from the arm's base. Move commands are not preformed in this function.
// Returns error codes based on if requested location exceeds working envelop.
// Codes are: 0 - no error, 1  - request point exceeds robot's physical reach, 
// 2 - requested point too close to robot's base.
int InverseKinematicsMath(double rValue, double hValue, double &shoulderAngle, double &elbowAngle) {
    double radius;
    double radiusSquared;
    double compoundShoulderAng1;
    double compoundShoulderAng2;

    radiusSquared = rValue*rValue+hValue*hValue;
    radius = sqrt(radiusSquared);

    // Check to see if requested position is out of range.
    if (radius>lowerArmLength+upperArmLength || radius>maxEnvelopRadius) {
        return 1;
    }
    if (radius <= 0 || radius<minEnvelopRadius) {
        return 2;
    }
    radius = radius - ShoulderOffset;
    elbowAngle=acos((lowerArmLength*lowerArmLength+upperArmLength*upperArmLength-radiusSquared)
        /(2*lowerArmLength*upperArmLength));

    compoundShoulderAng1= asin((upperArmLength*sin(elbowAngle))/radius);
    compoundShoulderAng2= atan(hValue/rValue);

    shoulderAngle=(compoundShoulderAng1+compoundShoulderAng2)*180/M_PI;
    elbowAngle=elbowAngle*180/M_PI;
 
  return 0;
}

// Function converts a requested xyz location into a semi-polar set of 
// coordinates. Breaks requested points into an azimuth angle and determines
// the 2D coordinates of requested point in the plane correponding to that
// azimuth plane of action. 
int ConvertToPolar(double x, double y, double z, double &rValue, double &hValue, double &aValue) {
    // Function returns an error.

    // hValue is direclty equivalent to z-axis
    hValue = z;
    if (y<0) {
        return 1;   // if requested y is below origin, not physically possible
    }
    rValue = sqrt(x*x+y*y); // r value, or radial distance is hypotenus of x and y
    
    // Due to the singularity in atan, set up cases for azimuth angle calculation
    // Angle provided in degrees (atan returns radians)
    if (x==0) {
        aValue=90;
    }else if (x>0){
        aValue=atan(y/x)*180/M_PI;
    }else {
    // the case of x<0
        aValue=180+atan(y/x)*180/M_PI;
    }
    return 0;
} 

// Primary call function. Provided a set of x,y,z coordinates, this function
// performs all inverse kinematics, returns joint angles, and moves the joints
// to reach the requested position. 
int MoveIK(double x, double y, double z, double &shoulderAngle, double &elbowAngle, double &baseAngle){
    double rValue;
    double hValue;
    double aValue;
    int errorCode;

    // Convert cartesian coordinates to polar
    errorCode = ConvertToPolar(x,y,z,rValue,hValue,aValue);
    // Using polar coordinates, perform the mathematics to return shoulder and elbow angle
    if (errorCode != 0){
        Serial.print("C2P Error");
        return errorCode;
    }
    errorCode = InverseKinematicsMath(rValue,hValue,shoulderAngle,elbowAngle);
    if (errorCode != 0){
        Serial.print("IK Error");
        return errorCode;
    }

    // Set the angle of the base
    baseAngle = aValue;

    // In each servo object, set the current angle
    RobotServos[ServoBase].currentAngle = baseAngle;
    RobotServos[ServoShoulder].currentAngle = shoulderAngle;
    // The elbow is a special case. As the elbow is controlled from the shoulder,
    // it requires the additonon of the current shoulder angle to compensate
    RobotServos[ServoElbow].currentAngle = elbowAngle+RobotServos[ServoShoulder].currentAngle;
    
    // Move all servos, minus the claw
    for (int iServo = 0; iServo < degreesOfFreedom-1; iServo++) {
        ActuateServo(RobotServos[iServo], RobotServos[iServo].currentAngle);
    }
}