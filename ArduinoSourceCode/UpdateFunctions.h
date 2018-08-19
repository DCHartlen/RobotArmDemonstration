//  UpdateFunctions - Operation control for encoder-based opearting modes
//
//  Created by: D.C. Hartlen
//  Date:       19-Aug-2018
//  Updated by: 
//  Date:
//  
//  Contains operating functions for all encoder based controls. Known as update
//  functions as they are called at every control loop of the underlying source 
//  code (default 20 Hz) and they "update" the arm's position based on new 
//  encoder inputs. 

void updateCalibrationMode();
void updateDirectEncoderControl();
void updateCartesianEncoderControl();
