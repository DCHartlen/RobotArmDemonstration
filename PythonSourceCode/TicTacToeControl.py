import io
import numpy as np
from matplotlib import pyplot as plt
from picamera import PiCamera
from picamera.array import PiRGBArray
import cv2 as cv
import time
import GameAI as botAI
from CalibratePlayfield import CalibratePlayfield

class TicTacToeControl:
    # Define ratio of inner circle (gamepeice) to outer circle (playfield)
    ratioInnerOuter = 0.8

    # Grayscale threshold. 255 = white, 0 = black.
    # TODO: add interactive thresholding??? Maybe adaptive thresholding???
    gsThreshold = 100

    players = ['X', 'O']
    firstPlayer = 'X' # Robot plays as X

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
            newSquare = self.ScanSquares(imageCapture)
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
            newSquare = self.ScanSquares(imageCapture)
            # if new != desired: through error and try again
            print("BOT placed marker at {}".format(newSquare))
            self.gameBoard.showBoard()

            # Check to see if BOT ended the game before passing back to HUMAN
            if self.gameBoard.complete():
                break
        
        # Print message
        if self.gameBoard.winner() == 'O':
            print('BOT wins!')
        elif self.gameBoard.winner() == 'X':
            print('HUMAN wins!')
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
        gsCapture = cv.cvtColor(imageCapture,cv.COLOR_BGR2GRAY)
        newSymbolIndex = None
        for iRegion, region in enumerate(self.regionROIs):
            # Capture only the current region from the greyscale image
            currentROI = self.__GetImageROI__(gsCapture,region)
            currentROI = cv.GaussianBlur(currentROI, (7,7),0)

            # scan through each region looking for circles
            gamePiece = cv.HoughCircles(currentROI,cv.HOUGH_GRADIENT, 2, 
                self.maxInnerRadii[iRegion],
                maxRadius=self.maxInnerRadii[iRegion])

            # If a cirlce is found, determine if it is an X or 0
            if gamePiece is not None:
                tokenCenter= np.uint16(np.around(gamePiece[0,0,0:2]))
                # Look for symbol
                symbol = self.DetectSymbol(currentROI, tokenCenter)
                # place symbol in the gameboard
                foundFlag = self.__UpdateGameboard__(iRegion,symbol)
                if foundFlag == 1:
                    newSymbolIndex = iRegion

                # add circle to image for debug
                cv.circle(currentROI, (tokenCenter[0], tokenCenter[1]), 
                    int(gamePiece[0,0,2]), (0, 255, 0), 2)
            # If there is no piece in the square, print message
            # TODO: add logic to detect hands, say the loss of a circle
            # else:   
                # print("empty")

            # If debug is activated, print region to screen
            if self.debugFlag == True:
                cv.imshow('region',currentROI)
                cv.waitKey(0)

        return newSymbolIndex

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

    def __UpdateGameboard__(self,index,symbol):
        """
        Uses vectorized board. Checks to ensure the board is actually empty,
        then check to make sure the human player hasn't tried to pull a fast one
        and change peices around

        returns: 1 = new symbol found, 0 = no change, 0 = no change
        """
        currentRegionValue = self.gameBoard.squares[index]
        if currentRegionValue == None:
            self.gameBoard.addMarker(index,symbol)
            return 1
        if currentRegionValue == symbol:
            return 0
        else:
            print('CHEATER!!!!')
            return 0


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
    gameControl.RunGameManual()
