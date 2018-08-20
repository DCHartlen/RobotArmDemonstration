"""
TicTacToeControl.py

Created by: D.C. Hartlen
Date:       19-Aug-2018
Updated by: 
Date:

Contains one very large class named "TicTacToeControl". This class interfaces 
with a Raspberry Pi camera to capture images of the gameboard, detect and 
recognize symbols, and play three different gamemodes including: 1) Automatic
gameplay were robot arm places the computer's move, 2) Player moves for the
computer, but the computer recongizes the new piece, and 3) a debug mode where
the player moves for the computer and uses keyboard press to make computer take
a picture of the board. 

"""
import io
import numpy as np
from matplotlib import pyplot as plt
# pylint:disable=E0401
from picamera import PiCamera
from picamera.array import PiRGBArray
# pylint:enable=E0401
import cv2 as cv
import time
import GameAI as botAI
from CalibrateGameboard import CalibrateGameboard
import SerialInterface as coms

class TicTacToeControl:
    """ 
    Class which handles all symbol detection and gameplay
    Inputs:
        debugFlag (boolean): Passed to calibration module. If true, information
            regarding calibration is printed to terminal.
    """
    # Define ratio of inner circle (gamepiece) to outer circle (playfield)
    ratioInnerOuter = 0.8
    # Location of each marker inside the appropriate ROI
    markerLocations = [[None,None] for i in range(9)]
    # Grayscale threshold. 255 = white, 0 = black.
    # TODO: if a GUI is made, allow threshold to be changed interactively. 
    gsThreshold = 100
    # list players
    players = ['X', 'O']
    firstPlayer = 'X' # Human plays as X

    # Coordinates of the computers gamepiece bank. Specifically, lists containing
    # the (x,y,z) position of all pieces in the bank. 
    # TODO: set coordinates of the pieces bank.
    bankCoordinates = [
        [1, 1, 1],
        [2, 2, 1],
        [3, 3, 1],
        [4, 4, 1],
        [5, 5, 1],
    ]

    # Home coordinates for the robot arm. Out of the way of the camera. Maybe
    # over top the gamepiece bank?
    # TODO: set home coordinates
    homeCoordinates = [2,2,5]

    # Coordinates of all game squares. These coordinates are different from
    # calibration coordinates as they are in mm and relative to the robotic arm
    # TODO: Set position of each square.
    squareCoordinates = [
        [1, 1, 1],
        [1, 2, 1],
        [1, 3, 1],
        [2, 1, 1],
        [2, 2, 1],
        [2, 3, 1],
        [3, 1, 1],
        [3, 2, 1],
        [3, 3, 1],
    ]

    def __init__(self, debugFlag=False):
        print("started")
        self.debugFlag = debugFlag
        self.gameBoard = botAI.Game()
        self.camera = PiCamera()
        self.camera.resolution = (800,800)
        self.camera.rotation = 180
        PiRGBArray(self.camera) #FIXME: removed variable as it caused linting error. Required?

    def StartSerial(self, port=0):
        """ 
        Starts serial communication with arduino
        Inputs:
            port: com port with arduino activated        
        """
        # TODO: finish serial communications interface. Like setting comport?
        self.serial = coms.SerialInterface(port)

    def RunGame(self):
        """
        Start an automated game were the HUMAN and BOT move thier own peices.
        """
        # capturePeriod = 0.2 # in seconds
        nCaptures = 6       # number of stationary symbols before confimation
        iCaptures = 0       # iterations with stationary symbol
        imagePrototype = PiRGBArray(self.camera)
        lastIndex = None    # tracks last symbol for counter
        # TODO: Add argument that lets computer (O) go first. Human moves first
        # by default
        currentPlayer = self.firstPlayer
        # start opencv window thread which shows the current, annoted image
        cv.startWindowThread()
        cv.namedWindow("CamCapture")
        iTurn = 0  # Initialize move counter
        # Play game until completion. Only difference between HUMAN and BOT is
        # bot moves arm. Images are always being taken. Serial coms (MoveArm)
        # is threaded to have communication and movement occure concurrently.
        while not self.gameBoard.complete():
            if currentPlayer == 'X':
                print("HUMAN's turn. Please add a marker to the board...")

            else:
                print("BOT is moving...")
                botMove = botAI.determineMove(self.gameBoard,currentPlayer)
                print("BOT choose square {} as its next move".format(botMove))
                self.MoveArm(iTurn, botMove)
                # Increment move counter only after on BOT's
                iTurn+=1

            # look for new symbol. 
            while iCaptures<nCaptures:
                # CaptureImage
                imagePrototype = PiRGBArray(self.camera)
                self.camera.capture(imagePrototype, format='bgr')
                imageCapture = imagePrototype.array
                # scan board for new symbols
                newIndex, symbol, newCenter = self.ScanSquares(imageCapture)
                # if no symbol found, reset confirmation counter
                if newIndex is None:
                    iCaptures = 0
                # if a symbol is found, run checks...
                else:
                    # If new symbol is the same as the last, increment counter
                    if newIndex == lastIndex:
                        lastIndex = newIndex
                        iCaptures += 1
                    # if the symbol is different, reset counter
                    else:
                        lastIndex  = newIndex
                        iCaptures = 0

                # Annotate and display image
                self.__ShowAnnotated__(imageCapture,newIndex, newCenter, symbol)
                # wait the proscribed period. process time may be significant,
                # a dedicated wait may not be needed.
                # time.sleep(capturePeriod) #FIXME: is a sleep timer needed?
            
            # Outside the loop only occurs if a new peice is found
            print("New symbol found at {}".format(newIndex))
            # Add to currentPlayer's token to gameboard and show the new board
            self.gameBoard.addMarker(newIndex,currentPlayer)
            self.markerLocations[newIndex] = newCenter
            self.gameBoard.showBoard()
            # switch currentPlayer, reset counter
            currentPlayer = botAI.getEnemy(currentPlayer)
            iCaptures = 0
        
        # when the game completes, determine the winner
        winner, combo = self.gameBoard.declareWinner()
        self.__ShowAnnotated__(imageCapture)
        self.__DrawWinningLine__(imageCapture,combo)
        if winner == 'O':
            print('BOT wins!')
            print(combo)
        elif winner == 'X':
            print('HUMAN wins!')
            print(combo)
        else:
            print('Game tied...')

    def ManualGameInterupts(self):
        """
        Run a game with manual interupts. Hit enter or something to continue. 
        Deprecated, but useful for debug
        """
        # Loop while the game is not complete (controlled by AI)
        imagePrototype = PiRGBArray(self.camera)
        plt.ion()
        while not self.gameBoard.complete():
            player = 'X'
            input("HUMAN's turn. Hit enter to end")
            # capture image of board at the end of HUMAN turn
            imagePrototype = PiRGBArray(self.camera)
            self.camera.capture(imagePrototype,format='bgr')
            imageCapture = imagePrototype.array
            plt.imshow(imageCapture)
            plt.show()
            # look at board and find new piece
            newSquare, _, _ = self.ScanSquares(imageCapture)
            self.gameBoard.addMarker(newSquare,player)
            # ScanSquares returns the location of the new token and sets gameboard
            print("HUMAN placed marker at {}".format(newSquare))
            self.gameBoard.showBoard()

            # Check if HUMAN finished game before passing to BOT
            if self.gameBoard.complete():
                break

            print("BOT is moving")
            # BOT chooses new move
            player = 'O'
            botMove = botAI.determineMove(self.gameBoard,player)
            input("BOT chooses {}, Move for BOT and hit enter".format(botMove))
            # Take image and find new marker
            imagePrototype = PiRGBArray(self.camera)
            self.camera.capture(imagePrototype,format='bgr')
            imageCapture = imagePrototype.array
            plt.imshow(imageCapture)
            plt.show()

            # Look for new marker
            newSquare, _, _ = self.ScanSquares(imageCapture)
            self.gameBoard.addMarker(newSquare,player)
            # if new != desired: through error and try again
            print("BOT placed marker at {}".format(newSquare))
            self.gameBoard.showBoard()

            # Check to see if BOT ended the game before passing back to HUMAN
            if self.gameBoard.complete():
                break
        
        # Print message
        winner, combo = self.gameBoard.declareWinner()
        if winner == 'O':
            print('BOT wins!')
            print(combo)
        elif winner == 'X':
            print('HUMAN wins!')
            print(combo)
        else:
            print('Game tied...')

    def ManualGame(self):
        """
        Run a manual game in which the BOT picks the next move, the HUMAN 
        move the pieces, and the BOT makes sure the piece is in the right spot.
        """
        # capturePeriod = 0.2 # in seconds
        nCaptures = 6       # number of stationary symbols before confimation
        iCaptures = 0       # iterations with stationary symbol
        imagePrototype = PiRGBArray(self.camera)
        lastIndex = None    # tracks last symbol for counter
        # Add argument that lets computer (O) go first. Human moves first by 
        # default
        currentPlayer = "X"
        # start opencv window thread which shows the current, annoted image
        cv.startWindowThread()
        cv.namedWindow("CamCapture")
        while not self.gameBoard.complete():
            if currentPlayer == 'X':
                print("HUMAN's turn. Please add a marker to the board...")
            else:
                print("Please move for BOT...")
                #TODO: add first move logic for bot

            # look for new symbol. This is blocking. Maybe run threaded? 
            while iCaptures<nCaptures:
                # CaptureImage
                imagePrototype = PiRGBArray(self.camera)
                self.camera.capture(imagePrototype, format='bgr')
                imageCapture = imagePrototype.array
                # scan board for new symbols
                newIndex, symbol, newCenter = self.ScanSquares(imageCapture)
                # if no symbol found, reset confirmation counter
                if newIndex is None:
                    iCaptures = 0
                # if a symbol is found, run checks...
                else:
                    # If new symbol is the same as the last, increment counter
                    if newIndex == lastIndex:
                        lastIndex = newIndex
                        iCaptures += 1
                    # if the symbol is different, reset counter
                    else:
                        lastIndex  = newIndex
                        iCaptures = 0

                # Annotate and display image
                self.__ShowAnnotated__(imageCapture,newIndex, newCenter, symbol)
                # wait the proscribed period. process time may be significant,
                # a dedicated wait may not be needed.
                # time.sleep(capturePeriod) #FIXME: is a sleep timer needed?
            
            # Outside the loop only occurs if a new peice is found
            print("New symbol found at {}".format(newIndex))
            # Add to currentPlayer's token to gameboard and show the new board
            self.gameBoard.addMarker(newIndex,currentPlayer)
            self.markerLocations[newIndex] = newCenter
            self.gameBoard.showBoard()
            # switch currentPlayer, reset counter
            currentPlayer = botAI.getEnemy(currentPlayer)
            iCaptures = 0
            # if the currentPlayer is now BOT/'O', run algorithm to determine
            # BOT's next move
            if currentPlayer == 'O':
                # Run a game complete check prior to bot picking move
                if self.gameBoard.complete():
                    break
                botMove = botAI.determineMove(self.gameBoard,currentPlayer)
                print("BOT choose square {} as its next move".format(botMove))
        
        # when the game completes, determine the winner
        winner, combo = self.gameBoard.declareWinner()
        self.__ShowAnnotated__(imageCapture)
        self.__DrawWinningLine__(imageCapture,combo)
        if winner == 'O':
            print('BOT wins!')
            print(combo)
        elif winner == 'X':
            print('HUMAN wins!')
            print(combo)
        else:
            print('Game tied...')

    def GameboardCalibration(self, imageCapture):
        """
        Using the CalibrateGameboard class, this function finds the circle
        centers and radii, and generates ROIs for each square. All values are
        self-assigned, not returned. 
        Inputs:
            imageCapture: RBG image of empty gameboard
        Self-Assigns:
            regionCenters: Centers of each square
            outerRadii: the radii of the outer gameboard circles in square
            regionROIs: the image ROIs for each square
            maxInnerRadii: the maximum gamepiece size expected. Used for circle
                detection. Based on each square's size to deal with persepctive
        """
        # Create calibration object
        cal = CalibrateGameboard(self.debugFlag)
        # return the region location and outerRadii.
        self.regionCenters, self.outerRadii = cal.FindPlayCircles(imageCapture)
        # return teh ROIs for each image
        self.regionROIs = cal.CreateROIs(self.regionCenters, self.outerRadii)
        # using the outerRadii, caluclate max inner radii for circle detect
        self.maxInnerRadii = np.uint16(np.around(self.outerRadii*
                                                 self.ratioInnerOuter))
    
    def ScanSquares(self,imageCapture):
        """
        Given an image capture, Look for gamepieces. Returns any new pieces
        that are found while ignoring old ones. Note: assumes only one new
        pieces has been added. 
        Inputs:
            imageCapture:  color image of gameboard with pieces
        Returns:
            newSymbolIndex: the square where the new piece is found (int)
            symbol: is the new piece 'X' or 'O'
            newCenter: the global coordinates of the new piece
        """
        # Convert image to grey scale
        gsCapture = cv.cvtColor(imageCapture,cv.COLOR_BGR2GRAY)
        # initialize returned variables to None
        newSymbolIndex = None
        symbol = None
        newCenter = None
        # Iterate through all ROIs to find circles
        for iRegion, region in enumerate(self.regionROIs):
            # Capture only the current region from the greyscale image
            currentROI = self.__GetImageROI__(gsCapture,region)
            currentROI = cv.GaussianBlur(currentROI, (7,7),0)

            # scan each region looking for circles
            gamePiece = cv.HoughCircles(currentROI,cv.HOUGH_GRADIENT, 2, 
                self.maxInnerRadii[iRegion],
                maxRadius=self.maxInnerRadii[iRegion])

            # If a cirlce is found, determine if it is an X or 0
            if gamePiece is not None:
                tokenCenter= np.uint16(np.around(gamePiece[0,0,0:2]))
                # Look for symbol
                symbol = self.DetectSymbol(currentROI, tokenCenter)
                # place symbol in the gameboard
                foundFlag = self.__CheckNew__(iRegion,symbol)
                if foundFlag == 1:
                    newSymbolIndex = iRegion
                    newCenter = tokenCenter
        # If no circle is found, these variables return None            
        return newSymbolIndex, symbol, newCenter

    def DetectSymbol(self, currentROI, tokenCenter):
        """
        If a game piece is found in an ROI, determine if it is 'X' or 'O'. 
        Pieces with and 'X' will have a white center while pieces with an 'O' 
        will have a coloured/grey center.
        Inputs:
            currentROI: the Current image subset
            tokenCenter: ROI coordinates of the gamepieces center. 
        Returns:
            'X' or 'O'
        """
        # Get the average of a [6,6] pixel array at the center of the gamepiece
        subset = currentROI[(tokenCenter[1]-3):(tokenCenter[1]+3),
                            (tokenCenter[0]-3):(tokenCenter[0]+3)]
        # Compute the overall mean value of the pixels. 
        subsetAvg = np.mean(np.mean(subset))
        # Base on average pixel value, determine if X or O. X has white center,
        # O has a grey center
        if subsetAvg > self.gsThreshold:
            # print('FOUND: X!!!')
            return 'X'
        else:
            # print('FOUND: O!!!')
            return 'O'

    def MoveArm(self, currentMove, target):
        """
        Places a gamepiece from the bank onto the gameboard.
        Inputs:
            currentMove: used to pic peices from the bank
            target: position on the gameboard
        """
        #TODO: Currently a blocking operation. make threaded?
        clawOpen = 85   # Open position of the manipulator
        clawClose = 45  # Closed position of manipulator
        
        bankLoc = self.bankCoordinates[currentMove]
        targetLoc = self.squareCoordinates[target]

        # TODO: are pauses required between moves?
        # Move claw to piece in bank, open, lower, close, raise
        # Move to, stay above
        cmd = coms.GenerateMoveCommand(bankLoc[0], bankLoc[1], bankLoc[2]+50)
        self.serial.SendGcode(cmd)
        # Open Claw
        cmd = "M106 {}".format(clawOpen)
        self.serial.SendGcode(cmd)
        # Lower
        cmd = coms.GenerateMoveCommand(bankLoc[0], bankLoc[1], bankLoc[2])
        self.serial.SendGcode(cmd)
        # Close claw
        cmd = "M106 {}".format(clawClose)
        self.serial.SendGcode(cmd)
        # Raise
        cmd = coms.GenerateMoveCommand(bankLoc[0], bankLoc[1], bankLoc[2]+50)
        self.serial.SendGcode(cmd)

        # Move to targeted game square, lower, open, raise, 
        # Move to, stay above
        cmd = coms.GenerateMoveCommand(targetLoc[0], targetLoc[1], targetLoc[2]+50)
        self.serial.SendGcode(cmd)
        # Lower
        cmd = coms.GenerateMoveCommand(targetLoc[0], targetLoc[1], targetLoc[2])
        self.serial.SendGcode(cmd)
        # Open claw
        cmd = "M106 {}".format(clawOpen)
        self.serial.SendGcode(cmd)
        # Raise
        cmd = coms.GenerateMoveCommand(targetLoc[0], targetLoc[1], targetLoc[2]+50)
        self.serial.SendGcode(cmd)

        # Move to home position
        cmd = coms.GenerateMoveCommand(self.homeCoordinates[0],
                                       self.homeCoordinates[1],
                                       self.homeCoordinates[2])
        self.serial.SendGcode(cmd)

    def __GetImageROI__(self,imageCapture,region):
        """
        Given an image capture and ROI region bounds, return an image subset
        Inputs:
            imageCapture
            region: bounds of the image [yMin,yMax,xMin,xMax]
        Returns:
            roi: the image subset
        """
        roi = imageCapture[region[0]:region[1], region[2]:region[3]]
        return roi

    def __CheckNew__(self,index,symbol):
        """
        Uses vectorized board. Checks to ensure the board is actually empty,
        then check to make sure the human player hasn't tried to pull a fast one
        and change peices around

        returns: 1 = new symbol found, 0 = no change, 0 = no change
        """
        currentRegionValue = self.gameBoard.squares[index]
        if currentRegionValue == None:
            return 1
        if currentRegionValue == symbol:
            return 0
        else:
            # print('CHEATER!!!!')
            return 0
    
    def __ShowAnnotated__(self,imageCapture,tempIndex=None,tempLocation=None,tempSymbol=None):
        """
        Generates annotations for the image displayed on the screen. Circles
        all set and transient gamepieces with different color circles and X's.
        Inputs:
            imageCapture: The color image of the current gameboard config
            tempIndex: Used to markup transient piece.
            tempLocation: Used to markup transient piece.
            tempSymbol: Used to markup transient piece.
        """

        # Copy board and locations for use in this function. Note, due to python
        # variable management, if copy() isn't used, a "pointer" is passed which
        # alter the class variable. Copy() ensure class variable isn't altered
        tempBoard = self.gameBoard.squares.copy() # create temporary gameboard
        markerLocations = self.markerLocations.copy()
        # If there is a region inputted and the corresponding region is empty,
        if tempIndex is not None:
            if tempBoard[tempIndex] is None:
                tempBoard[tempIndex] = tempSymbol
                markerLocations[tempIndex] = tempLocation
        
        # for all markers on the board, draw lines. Does not update location of
        # old markers. However the current marker does update.
        for i,e in enumerate(markerLocations):
            if tempBoard[i] == 'O':
                centerX = self.regionROIs[i][2]+e[0]
                centerY = self.regionROIs[i][0]+e[1]
                cv.circle(imageCapture, 
                    (centerX, centerY),
                    int(0.70*self.outerRadii[i]), [0, 0, 225], 4)
            if tempBoard[i] == 'X':
                centerX = self.regionROIs[i][2]+e[0]
                centerY = self.regionROIs[i][0]+e[1]
                radii = int(0.70*self.outerRadii[i])
                # offset for creating inscribed X
                offset = int(np.sqrt(0.5*radii*radii))
                cv.circle(imageCapture, 
                    (centerX, centerY),
                    radii, [0, 225, 0], 4)
                cv.line(imageCapture,(centerX-offset, centerY-offset),
                    (centerX+offset, centerY+offset),[0, 225, 0], 6)
                cv.line(imageCapture,(centerX+offset, centerY-offset),
                    (centerX-offset, centerY+offset),[0, 225, 0], 6)
        # Display image in named window which is being run threaded.
        cv.imshow("CamCapture",imageCapture)

    def __DrawWinningLine__(self, imageCapture, combo):
        """
        Draws the winning line
        Inputs:
            imageCapture: the color image capture
            combo: The combo that one the game
        """
        pts = [[None, None],[None, None]]
        if combo == [0, 1, 2]:
            pts[0] = [self.regionROIs[0][2],
                    int(0.5*(self.regionROIs[0][0]+self.regionROIs[0][1]))]
            pts[1] = [self.regionROIs[2][3],
                    int(0.5*(self.regionROIs[2][0]+self.regionROIs[2][1]))]

        elif combo == [3, 4, 5]:
            pts[0] = [self.regionROIs[3][2],
                    int(0.5*(self.regionROIs[3][0]+self.regionROIs[3][1]))]
            pts[1] = [self.regionROIs[5][3],
                    int(0.5*(self.regionROIs[5][0]+self.regionROIs[5][1]))]

        elif combo == [6, 7, 8]:
            pts[0] = [self.regionROIs[6][2],
                    int(0.5*(self.regionROIs[6][0]+self.regionROIs[6][1]))]
            pts[1] = [self.regionROIs[8][3],
                    int(0.5*(self.regionROIs[8][0]+self.regionROIs[8][1]))]

        elif combo == [0, 3, 6]:
            pts[0] = [int(0.5*(self.regionROIs[0][2]+self.regionROIs[0][3])),
                    self.regionROIs[0][0]]
            pts[1] = [int(0.5*(self.regionROIs[6][2]+self.regionROIs[6][3])),
                    self.regionROIs[6][1]]

        elif combo == [1, 4, 7]:
            pts[0] = [int(0.5*(self.regionROIs[1][2]+self.regionROIs[1][3])),
                    self.regionROIs[1][0]]
            pts[1] = [int(0.5*(self.regionROIs[7][2]+self.regionROIs[7][3])),
                    self.regionROIs[7][1]]

        elif combo == [2, 5, 8]:
            pts[0] = [int(0.5*(self.regionROIs[2][2]+self.regionROIs[2][3])),
                    self.regionROIs[2][0]]
            pts[1] = [int(0.5*(self.regionROIs[8][2]+self.regionROIs[8][3])),
                    self.regionROIs[8][1]]

        elif combo == [0, 4, 8]:
            pts[0] = [self.regionROIs[0][2], self.regionROIs[0][0]]
            pts[1] = [self.regionROIs[8][3], self.regionROIs[8][1]]

        elif combo == [2, 4, 6]:
            pts[0] = [self.regionROIs[2][3], self.regionROIs[2][0]]
            pts[1] = [self.regionROIs[6][2], self.regionROIs[6][1]]

        print(pts)
        cv.line(imageCapture,
            (pts[0][0],pts[0][1]), (pts[1][0],pts[1][1]), [0, 225, 225], 15)
        cv.imshow("CamCapture",imageCapture)

# Run the program if called directly from terminal/shell
if __name__ == "__main__":
    gameControl = TicTacToeControl()
    input('Clear game board and hit Enter to start...')
    test = PiRGBArray(gameControl.camera)
    time.sleep(0.1)
    gameControl.camera.capture(test,format='bgr')
    image = test.array
    gameControl.GameboardCalibration(image)
    cv.waitKey(1)
    print('HUMAN is X, BOT is O')
    gameControl.ManualGame()
    input('Hit Enter to end game...')
