"""
ExecuteGamepadControl.py

Created by: D.C. Hartlen
Date:       23-Aug-2018
Updated by: 
Date:

Standalone script allows the gamepad to interface with the robotic arm. Update 
frequency is 25 Hz.

"""

from GamepadInterface import GamepadInterface
import SerialInterface as si
from time import sleep

gamepad = GamepadInterface()
coms = si.SerialInterface(debugFlag=True)

delay = 1/25

# Clunky, but effect. Just push all commands. No provision for stopping.
while True:
    gamepadValues = gamepad.GetScaledAxes()
    coms.SendGamepadCommand(gamepadValues)
    sleep(delay)
