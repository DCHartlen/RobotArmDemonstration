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
from CalibratePlayfield import CalibratePlayfield

class TicTacToeControl:
    # Define ratio of inner circle (gamepeice) to outer circle (playfield)
    ratioInnerOuter = 0.8
    # Location of each marker inside the appropriate ROI
    markerLocations = [[None,None] for i in range(9)]
    # Grayscale threshold. 255 = white, 0 = black.
    # TODO: add interactive thresholding??? Maybe adaptive thresholding???
    gsThreshold = 100
    # list players
    players = ['X', 'O']
    firstPlayer = 'X' # Human plays as X

    def __init__(self, debugFlag=False):
        print("started")
        self.debugFlag = debugFlag
        self.gameBoard = botAI.Game()
        self.camera = PiCamera()
        self.camera.resolution = (800,800)
        self.camera.rotation = 180
        rawCapture = PiRGBArray(self.camera)

    def SetFirstPlayer(self,player='X'):
        self.firstPlayer = player

    def RunGameManual(self):
        """
        Run a game with manual interupts. Hit enter or something to continue. 
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
            # look at board and find new peice
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

    def RunGameAutomatic(self):
        """
        Run a game that does not require user interaction
        """
        capturePeriod = 0.2 # in seconds
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
                # time.sleep(capturePeriod) 
            
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

    def PlayfieldCalibration(self, imageCapture):
        # Create calibration object
        cal = CalibratePlayfield(self.debugFlag)
        # return the region location and outerRadii.
        self.regionCenters, self.outerRadii = cal.FindPlayCircles(imageCapture)
        # return teh ROIs for each image
        self.regionROIs = cal.CreateROIs(self.regionCenters, self.outerRadii)
        # using the outerRadii, caluclate max inner radii for circle detect
        self.maxInnerRadii = np.uint16(np.around(self.outerRadii*
                                                 self.ratioInnerOuter))
    
    def ScanSquares(self,imageCapture):
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

    def __GetImageROI__(self,imageCapture,region):
        # Given region bounds, return a small ROI for easy circle detect
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
            print('CHEATER!!!!')
            return 0
    
    def __ShowAnnotated__(self,imageCapture,tempIndex=None,tempLocation=None,tempSymbol=None):
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
        """ Draws the winning line"""
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
    gameControl.PlayfieldCalibration(image)
    cv.waitKey(1)
    print('HUMAN is X, BOT is O')
    gameControl.RunGameAutomatic()
    input('Hit Enter to end game...')
