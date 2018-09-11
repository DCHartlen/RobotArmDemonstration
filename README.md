# Robot Arm Demonstration

The goal of this project was to develop a low cost, 3D printed, robotic arm as
a demonstration piece to help promote STEM. To that end, the robotic arm was
supposed to able to be controlled by a gamepad, run pre-planned motions using a
g-code interpreter, and play Tic Tac Toe with a human play using computer
vision run off a Raspberry Pi with camera.

At present, the project is incomplete. While the majority of coding, including
arduino firmware, computer vision, and interface scripts, is complete, it has
not been fully debugged. As such, issues remain, primary in the interface
between various systems. Major issues are detailed below. This is not an
exhaustive list, as so little testing has been done.

# Overview of Current Issues

## Arduino Firmware
The major issue with the arduino firmware at present is the unusual behavior
when communicating gamepad commands via a serial connection with the Raspberry
Pi. When not connected, operating modes can be changed by double tapping the
encoder button as expected. However, once the serial communications connection
is made, the encoder ceases to function. Its is almost as if the serial
connection is blocking, reseting, or overriding the code which governs the
encoder's behavior. 

While disabled at present, there is a g-code interpreter included in the
Arduino firmware. This system is required for the arm to play Tic Tac Toe, with
a python script running on the Raspberry Pi feeding movement commands. However,
due to time constraints, operating modes for both direct g-code interpretation
and Tic Tac Toe have been commented out as they have not been tested at all. 

## Computer Vision & Tic Tac Toe
This is the most mature and functional portion of the project. All computer
vision is accomplished using OpenCV in python. While C++ would be faster,
particularly when run on a Raspberry Pi, Python is just so much easier to code
in and easier to interface with the Arduino powering the robotic arm. 

At present, this section of the code can play a game of Tic Tac Toe with a
human player. The system will recoginize game pieces, find new pieces, and
choose a new move based on the current game board configuration. The computer
will ask the human player to move for it. 

**The computer must play as 'O'. Further, the human player must go first as
'X'**

While code exists to allow the scripts to actuate the robotic arm to move
pieces, these have not been finished. They require debugging of Arduino g-code
interpreter, resolution of serial communications issue between python scripts
running the Raspberry Pi and the Arduino powering the robotic arm, and tuning
of the location of game squares on the gameboard.

## Gamepad Control
This feature is function, mostly. It does work, but results in jerky motion.
Part of this is due to the large amount of backlash present in the robotic arm,
but it is also due to the incremental updating of the servos position at every
timestep based on descrete motion commands. There is probably a more elegant
and smooth means of accomplishing this type of control, but time constraints
have not permitted exploration of this. 

An XBox 360 controller was used for all work. Other controllers may work, but
changes in how the joysticks are interpreted may result in different behavior. 

# Hardware and Required Software Packages

# Hardware
The robotic arm used for this project is the EEZYBotARM MK2 by daGHIZmo. It can
be found [here on Thingiverse]{https://www.thingiverse.com/thing:1454048}. Note
that the design may need to be modified to accomadate the servo horns provided
with your servos. I found that even though I bought the recommended servos
(which I do not recommend) the servo horns did not fit the cutouts in the
robotic arm. 

Control is accomplished using an Arduino Uno. An encoder with a pushbutton is
used to control operating modes as well as to manually actuate the robotic arm.
This project also uses a 4x20 LCD screen, communicating with I2C to the
Arduino, but this is entirely optional. 

## Arduino Firmware
The Arduino firmware provided for this project draws heavily from the [ThisArm
Firmware by Iamjonah]{https://github.com/lamjonah/ThisArm_Firmware/},
particularly the means of changing operating mode, the g-code interpreter, and
the inverse kinematics. 

Other required libraries:
- [Encoder]{https://github.com/PaulStoffregen/Encoder}: Used to track encoder
  rotation.
- [OneButton]{https://github.com/mathertel/OneButton}: controls encoder button
  debounce and allows for double-click behavior
- Servo: a built-in library used to control servos.
- LiquidCrystal_I2C: Used to interface with the 4x20 LCD screen via I2C
- Wire: built-in library used as the backend for LiquidCrystal_I2C

Links are included for the Encoder and OneButton Libraries, as they must be
installed seperately. The remaining libraries can be downloaded using the
Arduino IDE.

## Python Code
Python is used as the development environment for all Raspberry Pi based
compuation, including computer vision, serial interface, and gamepad interface.
The following libraries are required to continue development. All can be
downloaded from PYPI or using the Raspberry Pi application installer. 

- OpenCV: Used for all image recognition. This can be difficult to install on a
  Raspberry Pi
- PyGame: Used to interprete inputs from the Gamepad. This is not a very clean
  implimentation, but it does work, in its own way. 
- PySerial: Used to provide serial communicaitons with the Arduino.
- Numpy: Used for numerical computation

This project makes use of Tic Tac Toe AI created by [Cecil
Wobker]{https://cwoebker.com/posts/tic-tac-toe}. 