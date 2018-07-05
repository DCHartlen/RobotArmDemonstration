// Print all gcode commands to serial for reference
void help() {
  Serial.print(F("GcodeServoRobotArm "));
  Serial.println(F("Commands:"));
  Serial.println(F("G00 [X/Y/Z/E(mm)] ; - Instance move"));
  Serial.println(F("G01 [X/Y/Z/E(mm)] [F(mm/s)]; - linear move"));
  Serial.println(F("G04 P[seconds]; - delay"));
  Serial.println(F("G90; - absolute mode"));
  Serial.println(F("G91; - relative mode"));
  
  
  Serial.println(F("M100; - this help message"));
  Serial.println(F("M106 [S(Claw angle)]; - set angle of Claw"));
  Serial.println(F("M114; - report position and feedrate"));
  Serial.println(F("All commands must end with a newline."));
}

// Given a code (G, X, Y, Z, etc), find the assocaited value
float ParseCommandComponent(char code,float val) {
  char *ptr=serialBuffer;
  while(ptr && *ptr && ptr<buffer+currentBufferLength) {
    if(*ptr==code) {
      return atof(ptr+1);
    }
    ptr=strchr(ptr,' ')+1;
  }
  return val;
} 

// pauses but also prints duration of pause to serial
void pause(long ms) {
  Serial.println(ms);
  delay(ms);
}


// Shorthand means of printing coordinate pairs to serial
void outputCode(char *code,float val) {
  Serial.print(code);
  Serial.print(val);
  Serial.print(" ");
}

// Prints the current location to servo
void printCurrentLocation() {
  outputCode("X",X);
  outputCode("Y",Y);
  outputCode("Z",Z);
  outputCode("C",AngularServos[ServoClaw].Angle);
  outputCode("F",FeedRate);
  Serial.println(mode_abs?"ABS":"REL");
} 

// TODO add setpoint commands, fastest and feedrate.



// Parse buffer and execute setpoints
void ExecuteCommand() {
    float xTarget, yTarget, zTarget;

    int errorCode = 0;
    int command = ParseCommandComponent('G',-1);
    // All move commands have two modes. Absolute and relative. Relative adds
    // displacement to current location. Recommend always using absolute.
    switch(command) {
        // Case G00: move arm as fast as possible to new target location
        case  0:{
            if (mode_abs==1){
            xTarget=ParseCommandComponent('X',x);
            yTarget=ParseCommandComponent('Y',y);
            zTarget=ParseCommandComponent('Z',z);
            }else {
            xTarget=x+ParseCommandComponent('X',0);
            yTarget=y+ParseCommandComponent('Y',0);
            zTarget=z+ParseCommandComponent('Z',0);
            }

            // Move the arm
            rc=setpoint(xTarget, yTarget, zTarget);
            printCurrentLocation();
            break;
        }
        // Case G01: move arm at provided "feedrate"
        case  1: { // line
            if (mode_abs==1){
            xTarget=ParseCommandComponent('X',x);
            yTarget=ParseCommandComponent('Y',y);
            zTarget=ParseCommandComponent('Z',z);
            }else {
            xTarget=x+ParseCommandComponent('X',0);
            yTarget=y+ParseCommandComponent('Y',0);
            zTarget=z+ParseCommandComponent('Z',0);
            }
            FeedRate=ParseCommandComponent('F',FeedRate);
            
            // Move the arm
            rc=LinearSetpoint(xTarget, yTarget, zTarget,FeedRate);
            printCurrentLocation();
            break;
        }
        // Case G04: wait a set time
        case  4:
            pause(ParseCommandComponent('P',0)*1000);  
            break;
        // Case G90: Set absolute movement mode
        case 90:
            mode_abs=1;  
            break;
        // Case 91: Set relative movement mode
        case 91:  
            mode_abs=0;  
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
            ActuateServo(CommandServos[ServoClaw], ParseCommandComponent('S',AngularServos[ServoClaw].Angle));
            break;
        // Case M114: Print current location
        case 114:  
            printCurrentLocation();  
            break;
        default:  
            break;
    }

    
}

// Main entry point for Gcode control.  Reads serial buffer.
void GCodeControl(){
    // listen for serial commands from computer
    while(Serial.available() >0) {
        // Reading serial port one character at a time
        char c=Serial.read();
        // Serial.print(c); //echo for confirmation
        // add to character to buffer
        if(currentBufferLength<BufferLength-1) {
            serialBuffer[currentBufferLength++] = c;
        }
        // if a newline is found, 
        if (c == '\n') {
            serialBuffer[currentBufferLength] = 0;
            //TODO add ExecuteCommand
            //TODO add idle function
        }
    }

}