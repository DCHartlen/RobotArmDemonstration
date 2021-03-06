//  Arduino Source Code for a 3+1 degree of freedom robotic arm. 
//
//  This file is the main entry point for compilation.
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//
//  Source code is designed to very modular to account for differenting 
//  operating modes. As a result, this file is primarily concerned with updating
//  and IO control. Details of specific operating modes are found in the various
//  librarys. Operating modes (in order) are listed below. Bounds are enforced 
//  unless otherwise noted. 
//    1) Calibration: Direct control of servo PWM. Bounds are not enforced
//    2) Direct Encoder Control: Control angle of each joint using a single 
//         encoder input. 
//    3) Cartesian Encoder Control: Control the [x,y,z] position of manipulator
//         using a single encoder input.
//    4) G-Code Control: Manipulator responds to G-Code commands send by UART
//         interface (generally a computer).
//    5) Direct Gamepad Control: Joints are individually controlled by an XBox
//         controller plugged into a computer. Commands send by UART.
//    6) Cartesian Gamepad Control: [x,y,z] position of manipulator is 
//         controlled with an XBox controller. Commands sent by UART, as above.
//    7) Tic Tac Toe: Robot arm responds to move commands to play Tic Tac Toe. 
//         Note: All game logic is conducted off the arduino. Arduino merely
//         responds to move commands, just like the G-Code mode. 
//
//  Robot geometry (required for g-code and cartesian control) is defined in 
//  "Kinematics.h"
//
//  Source code works on an Arduino Uno with no lag, but does fill approximately
//  65-75% of the Arduino's ROM and RAM. Faster microcontrollers with larger
//  storage and RAM can be used, but smaller ones will probably not work. 
//
//  This source code is based on the "ThisArm" firmware developed by "IamJonah".
//  https://github.com/lamjonah/ThisArm_Firmware/
//
//  This firmware is provided as is, with all liability assumed by the end user. 

#include <Encoder.h>    // Encoder controls
#include <Servo.h>      // Servo Controlls
#include <OneButton.h>  // Button debounce and double click controls
#include <Wire.h>       // I2C used for lcd screen
#include <LiquidCrystal_I2C.h>          // Control LCD screen
#include "ServoFunctions.h" // Defines each servo used in arm
#include "InitializationFunctions.h"    // defines all mode setup functions
#include "UpdateFunctions.h"// Defines controller updates for all operating modes
#include "GCodeModule.h"    // gcode parser
#include "GamepadInterface.h"           // gamepad interface and controls
#include "LCDMessages.h"    // Messages printed to the LCD screen
#include "SerialDebug.h"    // debug messages printed to serial monitor.
#include "OperatingModeDefinition.h"    // defines all operating modes
#include "Kinematics.h"     // Math for inverse kinematics

// Define digital pins for encoder. Encoder A and B pins must be connected
// to digital pins with interupt capacity, or encoder will not respond properly.
#define encoderPinA   2
#define encoderPinB   3
#define encoderPinBtn 12

// define encoder object
Encoder controlEncoder(encoderPinA,encoderPinB);

// define debounce object for encoder button
OneButton encoderBtn(encoderPinBtn, true);

// Define loop timing information (freq and timers)
unsigned long currentMillis = 0;        // Tracks current time
unsigned long lastEncoderMillis = 0;    // Time of last encoder timer
unsigned long lastUpdateMillis = 0;     // Time of last control update
unsigned long lastSerialMillis = 0;     // Time of last Serial update
unsigned long lastLcdMillis = 0;        // TIme of last LCD update
int encoderFreq = 2;    // Period at which encoder is polled (2ms)
int updateFreq = 20;    // Period at which commands are updated (10ms)
int serialFreq = 100;   // Period at which serial update (40ms)
int lcdFreq = 333;      // Period at which LCD is updated (200ms)  

// Define mode and joint counters
int currentMode = 0;  // Defines operating mode that system boots into at startup
int currentJointControlled = 0; // Defines joint currently controlled by encoder

// Define joint angles for arm
double baseAngle = 90;
double shoulderAngle = 90;
double elbowAngle = 90;
double clawAngle = 45;

// Define cartesian coordinates
double x = 0;
double y = 0;
double z = 0;

void setup() {
    Serial.begin(57600);    // define baud rate for USB serial coms

    // Define pin for encoder button
    pinMode(encoderPinBtn,INPUT_PULLUP); // Define button functionality

    // Define constants for encoder button to acknowlege click and double Click
    encoderBtn.setDebounceTicks(20);
    encoderBtn.setClickTicks(300);
    // These two lines attaches external interupts which define actions for clicking
    encoderBtn.attachDoubleClick(doubleClick);
    encoderBtn.attachClick(singleClick);

    // Initialize servos
    setupControlServos();
    // Initialize operating mode
    setupOperatingModes();
    // "Boot" into whatever mode is specified by currentMode
    OperatingModes[currentMode].ModeInitialization();

    // Setup LCD screen
    Screen.init();
    Screen.backlight();
    Screen.clear();
    // Pause for effect?
    delay(2000);
}

void loop() {
    currentMillis = millis();
    // Define a high speed loop which track encoder ticks
    if ((currentMillis-lastEncoderMillis) > encoderFreq) {
        lastEncoderMillis = currentMillis;  // update time tracker for next iteration
        // update the button click object. Needed to detect single and double clicks
        encoderBtn.tick(); 
    }

    // Define the control update loop
    if ((currentMillis-lastUpdateMillis) > updateFreq) {
        lastUpdateMillis = currentMillis;
        // Run update function for the given operating mode
        OperatingModes[currentMode].ModeUpdate();
    }

    // Define the serial update loop
    if ((currentMillis-lastSerialMillis) > serialFreq) {
        lastSerialMillis = currentMillis;
        // print data to serial as appropriate for given mode
        OperatingModes[currentMode].ModeSerialDebug();
    }

    // Define slow speed LCD update to avoid flicker
    if ((currentMillis-lastLcdMillis) > lcdFreq) {
        lastLcdMillis = currentMillis;
        // Update the lcd screen
        OperatingModes[currentMode].LCDMessageUpdate();
    }
}

void doubleClick() {
    currentMode++;  // Increment Mode
    currentMode = currentMode % nModes;   // modulus to ensure mode is always within available number
    OperatingModes[currentMode].ModeInitialization(); // Run mode initialization
    currentJointControlled = 0; // Reset joint control
    Screen.clear(); // Clear the lcd screen
}

void singleClick() {
    currentJointControlled++;   // Cycle through joints to cotrol. Same modulus control above.
    currentJointControlled = currentJointControlled % degreesOfFreedom;
}