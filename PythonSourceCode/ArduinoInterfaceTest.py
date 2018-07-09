# Test file only. Will be replaced/repurposed with more extensive arduino interface

import serial   # serial communications with arduino
import serial.tools.list_ports
from GamePadControl import GamePadControl # Gamepad interface
from time import sleep

class ArduinoInterface():
    # set default arduino com port to be the last one 
    # TODO: verify this works on raspberry pi
    # FIXME: Why the fuck did the com port order change?
    availablePorts = serial.tools.list_ports.comports()
    nPorts = len(availablePorts)
    arduinoPort = availablePorts[0].device
    

    ackSymbol = '>'.encode('utf-8')

    def __init__(self):
        print('Order of available comports')
        i = 1
        for e in self.availablePorts:
            print("{}: {}".format(i,e))
            i+=1
        self.ConnectArduino()
        # self.arduinoInput.flushInput()

    def ConnectArduino(self):
        print('Trying to connect...',end = ' ')
        try:
            self.arduinoInput = serial.Serial(self.arduinoPort, 9600, timeout=5)
            # Need to readline after initialization or the wprogram stalls later?
            self.arduinoInput.read_until(self.ackSymbol)
            print('Success!')
        except:
            # If there is an exception, return without starting collection
            print("Connection Failed. Check Port and Arduino.")
            return()

    def SendCommand(self, command):
        strCommand = ' '.join(str(e) for e in command)
        strCommand +=  '\n'
        self.arduinoInput.write(strCommand.encode('utf-8'))
        ackMsg = self.arduinoInput.read_until(self.ackSymbol)
        print(ackMsg.decode('utf-8'),end='')


if __name__ == "__main__":
    gamePad = GamePadControl()
    coms = ArduinoInterface()
    print('going to loop')

    for i in range(1,200):    # Run for only 20 loops (10 sec)
        values = gamePad.GetScaledAxes()
        coms.SendCommand(values)
        sleep(0.1)






