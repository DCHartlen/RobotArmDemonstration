// Tokenizes move command and extracts the desired move command.
void ParseGamepadCommand(char* commandString, int ioArray[]) {
    char* seg;
    int iSeg = 0;
    // Tokenize array in order to read it in as ints
    // Need to run first token manually
    seg = strtok(commandString," ");
    // convert token to integer
    ioArray[iSeg] = atoi(seg);
    // now use a loop to go through the rest of the char array
    while (seg != NULL) {
        iSeg++;
        seg = strtok(NULL," ");
        ioArray[iSeg] = atoi(seg);
    }   
}

// Main entry point for direct gamepad control. Read continously from serial
void DirectGamepadControl(){
  int commandArray[degreesOfFreedom];
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
            // append zero to end to make arduino happy
            serialBuffer[currentBufferLength] = 0;

            // parse the character array into an array of ints.
            ParseGamepadCommand(serialBuffer,commandArray);

            // Actuate servos an increment based on gamepad 
            for(int i = 0; i < degreesOfFreedom; i++){
                RobotServos[i].currentAngle = RobotServos[i].currentAngle+
                (commandArray[i]/updateHz*2);
                ActuateServo(RobotServos[i], RobotServos[i].currentAngle);
            }
            // report ready for next command.
            ReportReady();
        }
    }
}

// Main entry point for direct gamepad control. Read continously from serial
void CartesianGamepadControl(){
  int commandArray[degreesOfFreedom];
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
            // append zero to end to make arduino happy
            serialBuffer[currentBufferLength] = 0;

            // parse the character array into an array of ints.
            ParseGamepadCommand(serialBuffer,commandArray);

            // increment xyz position.
            // NOTE: moveIK does not place bounds on xyz location
            x = x+(commandArray[0]/updateHz*4);
            y = y+(commandArray[1]/updateHz*4);
            z = z+(commandArray[2]/updateHz*4);
            RobotServos[ServoClaw].currentAngle = 
                RobotServos[ServoClaw].currentAngle+(commandArray[3]/updateHz*2);
            BasicMove(x,y,z);
            ActuateServo(RobotServos[ServoClaw],RobotServos[ServoClaw].currentAngle);
 
            // report ready
            ReportReady();
        }
    }
}

// Initialization function for direct joint control. Homes arm and readies UART
// communications.
void setupDirectGamepadControl(){
  Serial.println("Direct Gamepad Control Initialized");
  // move to home position
  LinearMove(0,80,0,1000);
  ReportReady();
}

// Initialization function for cartesian control. Homes arm and readies UART
// communications.
void setupCartesianGamepadControl(){
  Serial.println("Cartesian Gamepad Control Initialized");
  // move to home position
  LinearMove(0,80,0,1000);
  ReportReady();
}