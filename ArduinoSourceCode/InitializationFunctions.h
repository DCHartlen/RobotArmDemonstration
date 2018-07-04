// Operating Mode 1: Calibration, directly control raw value. DOES NOT RESPECT
// MAX AND MIN SAFETIES. This mode must be run upon construction to set setpoints
// that allow for linear mapping of raw values to real angles.
void setupCalibrationMode();
// Operating Mode 2: Control servo angle directly with encoder
void setupDirectEncoderControl();