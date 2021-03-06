// Print all gcode commands to serial for reference
void help() {
  Serial.print(F("GcodeServoRobotArm "));
  Serial.println(F("Commands:"));
  Serial.println(F("G00 [X/Y/Z(mm)] ; - Quick move"));
  Serial.println(F("G01 [X/Y/Z(mm)] [F(mm/s)]; - Speed controlled, linear move"));
  Serial.println(F("G04 P[seconds]; - delay"));
  Serial.println(F("G90; - absolute mode"));
  Serial.println(F("G91; - relative mode"));
  
  
  Serial.println(F("M100; - this help message"));
  Serial.println(F("M106 [S(Claw angle)]; - set angle of Claw"));
  Serial.println(F("M114; - report position and feedrate"));
  Serial.println(F("All commands must end with a newline."));
}

// Given a specific command component (G, M, X, Y, etc), return the associated 
// value. So for G01, this function is called five times to fully parse a
// command. However, this allows the function work with commands of various
// length and contents
float ParseCommandComponent(char code,float val) {
  char *ptr=serialBuffer;
  while(ptr && *ptr && ptr<serialBuffer+currentBufferLength) {
    if(*ptr==code) {
      return atof(ptr+1);
    }
    ptr=strchr(ptr,' ')+1;
  }
  return val;
} 

// Pauses all motion for specifed time period
void pause(long ms) {
  Serial.println(ms);
  delay(ms);
}


// Helper function used to print current parsed command components to serial.
void outputCode(String code,float val) {
  Serial.print(code);
  Serial.print(val);
  Serial.print(" ");
}

// Prints the current location to servo
void printCurrentLocation() {
  outputCode("X",x);
  outputCode("Y",y);
  outputCode("Z",z);
  outputCode("C",RobotServos[ServoClaw].currentAngle);
  outputCode("F",feedRate);
  Serial.println(flagAbsolute?"ABS":"REL");
} 

// Move manipulator to the targeted location as fast as possible. This function
// is callled by more advanced function to generate speed and path controlled
// motion by using it in a peicewise manner. 
int BasicMove (float xTarget, float yTarget, float zTarget) {
    int errorCode = 0;

    // perfor inverse kinematics
    MoveIK(xTarget, yTarget, zTarget, shoulderAngle, elbowAngle,baseAngle);
    // update internal variable tracking current location.
    x = xTarget;
    y = yTarget;
    z = zTarget;
    return 0;
}

// Moves manipulator to the target location in a straight line at a give feedrate.
// Relies on repeated calls of Move and delays. By standard gcode convention,
// feedrate/moveSpeed is given in millimeters per minute
int LinearMove(float xTarget, float yTarget, float zTarget, float moveSpeed) {
    int  nSegments;
    int segmentDelay = 10;
    float xDelta, yDelta, zDelta;
    float moveDistance;
    unsigned long segmentMillis;

    // Compute cartesian difference between target and current location
    xDelta = xTarget-x;
    yDelta = yTarget-y;
    zDelta = zTarget-z;

    // Compute the rectangular norm (distance) between target and current
    moveDistance = sqrt(xDelta*xDelta + yDelta*yDelta + zDelta*zDelta);
    // determine number of segments based on a 100 Hz update speed
    nSegments = moveDistance/moveSpeed*60*(1000/segmentDelay);

    // Move in little segmnets
    for (int iSegment = 1; iSegment <= nSegments; iSegment++) {
        segmentMillis = millis();
        BasicMove(x+xDelta/nSegments, y+yDelta/nSegments, y+yDelta/nSegments);
        // non-blocking way to ensure nothing interupts arm motion. 
        while((millis()-segmentMillis)<segmentDelay) {}; 
    }
    // Move the final segment
    BasicMove(xTarget,yTarget,zTarget);
}

// Given a command stored in the UART buffer, this function parses and exeutes
// the command.
void ExecuteCommand() {
    float xTarget, yTarget, zTarget;

    int errorCode = 0;
    int command = ParseCommandComponent('G',-1);
    // All move commands have two modes. Absolute and relative. Relative adds
    // displacement to current location. Recommend always using absolute.
    switch(command) {
        // Case G00: move arm as fast as possible to new target location
        case  0:{
            if (flagAbsolute==1){
            xTarget=ParseCommandComponent('X',x);
            yTarget=ParseCommandComponent('Y',y);
            zTarget=ParseCommandComponent('Z',z);
            }else {
            xTarget=x+ParseCommandComponent('X',0);
            yTarget=y+ParseCommandComponent('Y',0);
            zTarget=z+ParseCommandComponent('Z',0);
            }

            // Move the arm
            errorCode=BasicMove(xTarget, yTarget, zTarget);
            printCurrentLocation();
            break;
        }
        // Case G01: move arm at provided "feedrate"
        case  1: { // line
            if (flagAbsolute==1){
            xTarget=ParseCommandComponent('X',x);
            yTarget=ParseCommandComponent('Y',y);
            zTarget=ParseCommandComponent('Z',z);
            }else {
            xTarget=x+ParseCommandComponent('X',0);
            yTarget=y+ParseCommandComponent('Y',0);
            zTarget=z+ParseCommandComponent('Z',0);
            }
            feedRate=ParseCommandComponent('F',feedRate);
            
            // Move the arm
            errorCode=LinearMove(xTarget, yTarget, zTarget,feedRate);
            printCurrentLocation();
            break;
        }
        // Case G04: wait a set time
        case  4:
            pause(ParseCommandComponent('P',0)*1000);  
            break;
        // Case G90: Set absolute movement mode
        case 90:
            flagAbsolute=1;  
            break;
        // Case 91: Set relative movement mode
        case 91:  
            flagAbsolute=0;  
            break;
        default:  
            break;
    }
    
    // Parse M commands (help, reporting and the claw)
    command = ParseCommandComponent('M',-1);
    switch(command) {
        // Case M100: print the help message
        case 100:
            help();  
            break;
        // Case M106: Actuate the claw
        case 106: 
            ActuateServo(RobotServos[ServoClaw], ParseCommandComponent('S',RobotServos[ServoClaw].currentAngle));
            break;
        // Case M114: Print current location
        case 114:  
            printCurrentLocation();  
            break;
        default:  
            break;
    }
}

// Resets buffer length tracking in preperation for the next command and
// transmits acknowledgment and ready symbols. 
void ReportReady() {
    // reset buffer indexer
    currentBufferLength = 0;
    Serial.println(F("Ack"));   // send acknowledgment
    Serial.print(F(">"));   // standard symbol indicates machine ready for next command
}

// Control function used for G-code control. Called every update time period, 
// however, due to serial communications, this function is blocking, so update
// frequency is not not maintained in g-code mode. 
void GCodeControl(){
    // listen for serial commands from computer
    while(Serial.available() >0) {
        // Reading serial port one character at a time
        char c=Serial.read();
        Serial.print(c); //echo for confirmation
        // add to character to buffer
        if(currentBufferLength<bufferLength-1) {
            serialBuffer[currentBufferLength++] = c;
        }
        // if a newline is found, 
        if (c == '\n') {
            // write a zero to make arduino happy
            serialBuffer[currentBufferLength] = 0;
            // parse and execute gcode command
            ExecuteCommand();
            // report ready
            ReportReady();
        }
    }
}

// Initialization function for g-code control. Homes arm and sends ready signal
void setupGCodeControl(){
  Serial.println("Gcode Control initializated");
  // move to home position
  LinearMove(0,80,80,1000);
  printCurrentLocation();
  ReportReady();
}

// Initialization mode for Tic Tac Toe mode. Identical to g-code initialization
// as all move commands used for Tic Tac Toe are transmitted using g-code. 
void setupTicTacToeControl(){
  Serial.println("Tic Tac Toe Mode Activated. Awaiting Commands...");
  // move to home position
  LinearMove(0,80,80,1000);
  printCurrentLocation();
  ReportReady();
}