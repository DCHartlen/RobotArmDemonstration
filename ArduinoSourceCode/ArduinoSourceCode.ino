// Some header stuff fill in later

#include <Encoder.h>    // Encoder controls
#include <Servo.h>      // Servo Controlls
#include <OneButton.h>    // Button debounce controls
#include "InitializationFunctions.h"    // defines all mode setup functions
#include "UpdateFunctions.h"    // Defines controller updates for all operating modes
#include "ServoFunctions.h"   // Defines each servo used in arm
#include "OperatingModeDefinition.h"    // defines all operating modes
#include "Kinematics.h"     // Math for inverse kinematics

// Define digital pins for encoder
#define encoderPinA 11
#define encoderPinB 12
#define encoderPinBtn 13

// Define constants
long countEncoderTicks = 0; // counts number of encoder turns

// define encoder object
Encoder controlEncoder(encoderPinA,encoderPinB);

// define debounce object for encoder button
OneButton encoderBtn(encoderPinBtn, true);

// Define loop timing information (freq and timers)
unsigned long currentMillis = 0;    // Tracks current time
unsigned long lastEncoderMillis = 0;    // Time of last encoder timer
unsigned long lastUpdateMillis = 0;     // Time of last control update
unsigned long lastSerialMillis = 0;    // Time of last Serial update
int encoderFreq = 2;     // freqency at which encoder is polled (2ms)
int updateFreq = 10;      // frequency at which commands are updated (10ms)
int SerialFreq = 40;      // Freqncy at which serial update (40ms)

// Define mode and joint counters
int currentMode = 0;  // Defines operating mode that system boots into at startup
int currentJointControlled = 0; // Defines joint currently controlled by encoder

// Define joint angles for arm
double baseAngle = 90;
double shoulderAngle = 90;
double elbowAngle = 90;
double clawAngle = 45;

// // Define polar coordinates (used in IK and Gcode)
// double rValue;   // rValue
// double hValue;   // hValue
// double aValue;   // Azimuth

// Define cartesian coordinates
double x = 0;
double y = 0;
double z = 0;

void setup() {
    Serial.begin(57600);    // define baud rate for USB serial coms

    pinMode(encoderPinBtn,INPUT_PULLUP); // Define button functionality

    encoderBtn.setDebounceTicks(20);
    encoderBtn.setClickTicks(300);
    encoderBtn.attachDoubleClick(doubleClick);
    encoderBtn.attachClick(singleClick);

    setupOperatingModes();
    // TODO add OperatingModes[currentMode].ModeInitialization();

    // TODO Add initialization for servos, pot control, and operating modes

}

void loop() {
    currentMillis = millis();

    // Define a high speed encoder loop that tracks encoder button presses and turns
    if ((currentMillis-lastEncoderMillis) > encoderFreq) {
        lastEncoderMillis = currentMillis;  // update time tracker for next iteration

        // update the button click object. Needed to detect single and double clicks
        encoderBtn.tick(); 

    }

}

void doubleClick() {
    Serial.println("DOUBLE CLICK!!");
    currentMode++;  // Increment Mode
    currentMode = currentMode%nModes;   // modulus to ensure mode is always within available number
    OperatingModes[currentMode].ModeInitialization; // Run mode initialization
    currentJointControlled = 0; // Reset joint control
    // TODO add doubleclick logic, increment operating mode, run mode setup, etc.
}

void singleClick() {
    Serial.println("Single click");
    currentJointControlled++;   // Cycle through joints to cotrol. Same modulus control above.
    currentJointControlled = currentJointControlled%degreesOfFreedom;
}