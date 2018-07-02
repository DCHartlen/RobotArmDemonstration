// Some header stuff fill in later

#include <Encoder.h>    // Encoder controls
#include <Servo.h>      // Servo Controlls
#include <OneButton.h>    // Button debounce controls

// Define digital pins for encoder
#define encoderPinBtn 11
#define encoderPinA 12
#define encoderPinB 13

// Define constants
long countEncoderTurns = 0; // counts number of encoder turns

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
int operatingMode = 0;  // Defines initial and current operating mode

// Define polar coordinates (used in IK and Gcode)
double R;   // Radius
double H;   // Angle theta
double A;   // Axial/ Z axis

// Define cartesian coordinates
double X = 0;
double Y = 0;
double Z = 0;

void setup() {
    Serial.begin(57600);    // define baud rate for USB serial coms

    pinMode(encoderPinBtn,INPUT_PULLUP); // Define button functionality

    encoderBtn.setDebounceTicks(20);
    encoderBtn.setClickTicks(300);
    encoderBtn.attachDoubleClick(doubleClick);
    encoderBtn.attachClick(singleClick);

    // Add initialization for servos, pot control, and operating modes

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
}

void singleClick() {
    Serial.println("Single click");
}