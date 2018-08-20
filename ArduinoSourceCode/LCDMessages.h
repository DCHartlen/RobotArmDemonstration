//  LCDMessage - Controls what is displayed on connected LCD screen
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//
// Source code controls the writing of information to an LCD screen. This code 
// mandates the screen must be 20x4 characters and communicates with the
// Arduino using I2C communication. If your screen does not match these critera,
// you will need to adjust these commands accordingly.

// create LCD object used for I2C communications (address, columns, rows)
LiquidCrystal_I2C Screen(0x27,20,4);

// Functions for each operating mode display different messages.
void LCDCalibration();
void LCDDirectEncoder();
void LCDCartesianEncoder();
void LCDGCodeControl();
void LCDDirectGamepad();
void LCDCartesianGamepad();
void LCDTicTacToeControl();