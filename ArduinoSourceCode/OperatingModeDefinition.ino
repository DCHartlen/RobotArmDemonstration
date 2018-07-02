void setupOperatingModes() {
    // Mode 1 (0): directly control servo angle using encoder.
    OperatingModes[0].ModeName = "DirectEncoderControl";
    OperatingModes[0].ModeInitialization = setupDirectEncoderControl;
    OperatingModes[0].ModeUpdate = updateDirectEncoderControl;

    // TODO Add more functions
}

