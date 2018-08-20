"""
SerialInterface.py

Created by: D.C. Hartlen
Date:       19-Aug-2018
Updated by: 
Date:

Contains the class "SerialInterface", which handles all Serial (aka UART) 
communications with the robotic arm. This includes G-code sending as well as
gamepad control.  An additional function "GenerateGcode" exists outside this 
class and generates a G01 command given as of xyz coordinates and a speed.
Inputs:
    port (int): the serial port which the Arduino is attached to.
    debugFlag (boolean): If true, will print debug messages to terminal.

"""

import serial 
import serial.tools.list_ports
from GamepadControl import GamepadControl # Gamepad interface
from time import sleep

class SerialInterface():
    """
    Class designed to communicate serially with the robotic arm arduino. This
    class handles both gamepad commands as well as g-code.
    """

    # Default com port (on windwos) for Arduino is last one
    availablePorts = serial.tools.list_ports.comports()
    nPorts = len(availablePorts)
    arduinoPort = availablePorts[-1].device

    # This symbol is used by gcode sender and gamepad control to signify the 
    # robot arm is ready for a new command. 
    ackSymbol = '>'.encode('utf-8')

    def __init__(self,port=0, debugFlag=False):
        print('Order of available comports')
        for i,e in enumerate(self.availablePorts):
            print("{}: {}".format(i,e))
        self.ConnectArduino(port)
        self.debugFlag = debugFlag
        # self.arduinoInput.flushInput()

    def ConnectArduino(self, port):
        """
        Given a particular port, connect to arduino.
        Inputs:
            port (int): coms port arduino is connected to.
        """
        print('Trying to connect...',end = ' ')
        try:
            self.arduinoPort = self.availablePorts[port].device
            self.arduinoInput = serial.Serial(self.arduinoPort, 9600, timeout=5)
            # Need to readline after initialization or the wprogram stalls later?
            self.arduinoInput.read_until(self.ackSymbol)
            print('Success!')
        except:
            # If there is an exception, return without starting collection
            print("Connection Failed. Check Port and Arduino.")
            return()

    def SendGcode(self, gcodeCommand):
        """
        Sends a complete gcode string to the Arduino
        Inputs:
            gcodeCommand: Formatted Gcode string
        Returns:
            ackMsg: string of decoded message from serial coms
        """
        # Append newline to command if not one already
        if (gcodeCommand[-1] is not '\n'):
            gcodeCommand += '\n'
        # Send the command and wait for a response
        self.arduinoInput.write(gcodeCommand.encode('utf-8'))
        ackMsg = self.arduinoInput.read_until(self.ackSymbol)
        if self.debugFlag is True:
            print(ackMsg.decode('utf-8'),end='')
        return ackMsg.decode('utf-8')

    def SendGamepadCommand(self, command):
        """
        Sends a gamepad command string to the Arduino
        Inputs:
            Command: Formatted Gcode string
        Returns:
            ackMsg: string of decoded message from serial coms
        """
        strCommand = ' '.join(str(e) for e in command)
        strCommand +=  '\n'
        self.arduinoInput.write(strCommand.encode('utf-8'))
        ackMsg = self.arduinoInput.read_until(self.ackSymbol)
        if self.debugFlag is True:
            print(ackMsg.decode('utf-8'),end='')
        return ackMsg.decode('utf-8')

def GenerateMoveCommand(x, y, z, feed=1000):
    """
    Generates a formatted gcode command to move to a specific location.
    Inputs:
        x,y,z: absolute location to move to (mm)
        feed: movement rate (mm/min). Default 1000
    Returns:
        formatted gcode command string.
    """
    return "G01 X{} Y{} Z{} F{}".format(x,y,z,feed)

"""
Standalone Execution generates an arbitrary gcode string but also connects
the gamepad direct to the arduino. Debug purposes only. 
"""
if __name__ == "__main__":
    gamepad = GamepadControl()
    coms = SerialInterface(debugFlag=True)
    print('going to loop')
    print(GenerateMoveCommand(10.2, 82.25323, 123.23223, 10223))

    for i in range(1,200):    # Run for only 20 loops (10 sec)
        values = gamepad.GetScaledAxes()
        coms.SendGamepadCommand(values)
        sleep(0.1)






