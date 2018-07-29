// TODO: add headers

// create LCD object
LiquidCrystal_I2C Screen(0x27,20,4);

void LCDCalibration();
void LCDDirectEncoder();
void LCDCartesianEncoder();
void LCDGCodeControl();
void LCDDirectGamepad();
void LCDCartesianGamepad();
void LCDTicTacToeControl();    // TODO: is this one needed?