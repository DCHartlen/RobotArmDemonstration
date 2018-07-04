// Operating Mode 1: Calibration, directly control raw value. DOES NOT RESPECT
// MAX AND MIN SAFETIES
void updateCalibrationMode();
// Operating Mode 2: Control servo angle directly with encoder
void updateDirectEncoderControl();
// Operating Mode 2: Control Cartesian coordinates using encoder
void updateCartesianEncoderControl();
