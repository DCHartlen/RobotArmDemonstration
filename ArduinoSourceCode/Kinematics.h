//Robot Geometry
float lowerArmLength =  134.5;
float upperArmLength =  148.0;
float minEnvelopRadius =  15.0;
float maxEnvelopRadius =  115.0;

float ShoulderOffset = 10.0;     // Distance which shoulder is offset from radial center.

// given a set of polar coordinates, performs inverse kinematics and moves arm 
// manipulator to that point.
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

    //Serial.println (compoundShoulderAng1);
    //Serial.println (compoundShoulderAng2);

    shoulderAngle=(compoundShoulderAng1+compoundShoulderAng2)*180/M_PI;
    elbowAngle=elbowAngle*180/M_PI;
 
  return 0;
}

// Converts cartesian coordinates (xyz) into polar (machine space) coordinates (r,h,a)
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

// Full IK. Takes xyz and returns appropriate joint angles. Does not write to servo.
int MoveIK(double x, double y, double z, double &shoulderAngle, double &elbowAngle, double &baseAngle){
    double rValue;
    double hValue;
    double aValue;

    // Convert cartesian coordinates to polar
    ConvertToPolar(x,y,z,rValue,hValue,aValue);
    // Using polar coordinates, perform the mathematics to return shoulder and elbow angle
    InverseKinematicsMath(rValue,hValue,shoulderAngle,elbowAngle);

    // Set the angle of the base
    baseAngle = aValue;

    // In each servo object, set the current angle
    ControlServos[ServoBase].currentAngle = baseAngle;
    ControlServos[ServoShoulder].currentAngle = shoulderAngle;
    // The elbow is a special case. As the elbow is controlled from the shoulder,
    // it requires the additonon of the current shoulder angle to compensate
    ControlServos[ServoElbow].currentAngle = elbowAngle+ControlServos[ServoShoulder].currentAngle;
    
    // Move all servos, minus the claw
    for (int iServo = 0; iServo < degreesOfFreedom-1; iServo++) {
        ActuateServo(ControlServos[iServo], ControlServos[iServo].currentAngle);
    }
}