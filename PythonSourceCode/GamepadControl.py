#TODO: add documentation
# Import specifc modules from PyGame to read gamepad
from pygame import joystick as pgCont
from pygame import event as pgEvent
# pylint:disable=E0611
from pygame import init as pgInit
# pylint:enable=E0611
from time import sleep  # Used for pausing only

# TODO: Why is this not callable?
class GamepadControl:
    """
    Class handles all joystick control. All interface functions return a 4 
    element vector of the form [rx, ry, lx, ly], where r is the right joystick,
    l is the left joystick, and x and y are the axis. 

    NOTE: This is for an XBox360 controller. Not sure about others.
    """
    indexAxes = [0,1,4,3] # indices for the values of interest. Not sure what 2 is.
    axesTransform = [1,-1,1,-1] # pygame returns inverse y-axis. This fixes it
    

    # Initialize PyGame and find joystick/gamepad
    def __init__(self):
        pgInit()
        # Defaults to first gamepad
        self.gamePad = pgCont.Joystick(0) # Note irregular python indexing
        self.gamePad.init()  # Initialize gamepad

    # Gets the value of the axis, floatp[-1,1]
    def GetRawAxes(self):
        # get the current axis value (non-blocking)
        pgEvent.get()
        # Initialize output vector
        axesValues = [0,0,0,0]
        # Iterate through indexAxes ad retrieve values
        for i in range(len(self.indexAxes)):
            axesValues[i] = self.gamePad.get_axis(self.indexAxes[i])
            axesValues[i] = axesValues[i]*self.axesTransform[i]

        # return the axesValues (floats)
        return axesValues

    def GetScaledAxes(self):
        # Scales input from float[-1,1] to int[-3,3]
        axesValues = self.GetRawAxes()
        for i in range(len(self.indexAxes)):
            # Linear interpolation from [-1,1] to [-3.5,3.5]
            axesValues[i] = int((axesValues[i]+1)*(7)/(2)-3.5)
        # Return as integers, not floats
        return axesValues
    
# if called directly, for testing purposes.
# TODO: remove before integration
if __name__ == "__main__":
    gamePad = GamepadControl()

    for i in range(20):    # Run for only 20 loops (10 sec)
        values = gamePad.GetScaledAxes()
        strValues = ' '.join(str(e) for e in values)
        # Test printing a byte array
        print(strValues)
        sleep(0.5)
