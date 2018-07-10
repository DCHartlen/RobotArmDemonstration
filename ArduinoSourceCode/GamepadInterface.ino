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
                ControlServos[i].currentAngle = ControlServos[i].currentAngle+
                (commandArray[i]/updateHz*2);
                ActuateServo(ControlServos[i], ControlServos[i].currentAngle);
            }
            // report ready
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
            ControlServos[ServoClaw].currentAngle = 
                ControlServos[ServoClaw].currentAngle+(commandArray[3]/updateHz*2);
            BasicMove(x,y,z);
            ActuateServo(ControlServos[ServoClaw],ControlServos[ServoClaw].currentAngle);
 
            // report ready
            ReportReady();
        }
    }
}

void setupGamepadControl(){
  Serial.println("Gamepad Control Initialized");
  // move to home position
  LinearMove(0,80,0,1000);
  ReportReady();
}