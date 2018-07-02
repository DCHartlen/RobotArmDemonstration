
void setupControlServos(){
    // During calibration, move arm to the angle1 and angle 2 positions. Read in
    // actual raw value and set in the appropriate area below. 

    // Base servo
    ControlServos[ServoBase].Pin = 10;
    ControlServos[ServoBase].ServoMin = 654;
    ControlServos[ServoBase].ServoMax = 2400;
    ControlServos[ServoBase].raw1 = 1499;
    ControlServos[ServoBase].angle1 = 90; 
    ControlServos[ServoBase].raw2 = 2382;
    ControlServos[ServoBase].angle2 = 180; 
    ControlServos[ServoBase].initialAngle = 90;
    ControlServos[ServoBase].ServoObj.attach(ControlServos[ServoBase].Pin);
    ControlServos[ServoBase].ServoObj.write(ControlServos[ServoBase].initialAngle);
    ControlServos[ServoBase].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoBase].currentAngle = ControlServos[ServoBase].initialAngle;


    // Shoulder servo
    ControlServos[ServoShoulder].Pin = 9;
    ControlServos[ServoShoulder].ServoMin = 841;
    ControlServos[ServoShoulder].ServoMax = 2248;
    ControlServos[ServoShoulder].raw1 = 1887;
    ControlServos[ServoShoulder].angle1 = 180; 
    ControlServos[ServoShoulder].raw2 = 865;
    ControlServos[ServoShoulder].angle2 = 90; 
    ControlServos[ServoShoulder].initialAngle = 180;
    ControlServos[ServoShoulder].ServoObj.attach(ControlServos[ServoShoulder].Pin);
    ControlServos[ServoShoulder].ServoObj.write(ControlServos[ServoShoulder].initialAngle);
    ControlServos[ServoShoulder].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoShoulder].currentAngle = ControlServos[ServoShoulder].initialAngle;

        // Elbow servo
    ControlServos[ServoElbow].Pin = 6;
    ControlServos[ServoElbow].ServoMin = 600;
    ControlServos[ServoElbow].ServoMax = 2400;
    ControlServos[ServoElbow].raw1 = 2400;
    ControlServos[ServoElbow].angle1 = 0; 
    ControlServos[ServoElbow].raw2 = 1292;
    ControlServos[ServoElbow].angle2 = 90; 
    ControlServos[ServoElbow].initialAngle = 90;
    ControlServos[ServoElbow].ServoObj.attach(ControlServos[ServoElbow].Pin);
    ControlServos[ServoElbow].ServoObj.write(ControlServos[ServoElbow].initialAngle);
    ControlServos[ServoElbow].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoElbow].currentAngle = ControlServos[ServoElbow].initialAngle;

        // Claw servo
    ControlServos[ServoClaw].Pin = 5;
    ControlServos[ServoClaw].ServoMin = 800;
    ControlServos[ServoClaw].ServoMax = 2400;
    ControlServos[ServoClaw].raw1 = 2400;
    ControlServos[ServoClaw].angle1 = 0; 
    ControlServos[ServoClaw].raw2 = 1126;
    ControlServos[ServoClaw].angle2 = 90; 
    ControlServos[ServoClaw].initialAngle = 45;
    ControlServos[ServoClaw].ServoObj.attach(ControlServos[ServoClaw].Pin);
    ControlServos[ServoClaw].ServoObj.write(ControlServos[ServoClaw].initialAngle);
    ControlServos[ServoClaw].currentRaw = 1500; //TODO replace with linear map
    ControlServos[ServoClaw].currentAngle = ControlServos[ServoClaw].initialAngle;
}