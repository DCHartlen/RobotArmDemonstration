//Robot Geometry
float lowerArmLength = 80;
float upperArmLength = 80;
float minEnvelopRadius = 15;
float maxEnvelopEnvelop = 115;

// given a set of polar coordinates, performs inverse kinematics and moves arm 
// manipulator to that point.
int RunInverseKinematics(double rValue, double hValue, double &shoulderAngle, double &elbowAngle);

// Converts cartesian coordinates (xyz) into polar (machine space) coordinates (r,h,a)
int ConvertToPolar(double x, double y, double z, double &rValue, double &hValue, double &aValue )