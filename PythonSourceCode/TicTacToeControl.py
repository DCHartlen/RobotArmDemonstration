import numpy as np
import cv2 as cv
import GameAI as brainBox
from CalibratePlayfield import CalibratePlayfield

class TicTacToeControl:
    # Define ratio of inner circle (gamepeice) to outer circle (playfield)
    ratioInnerOuter = 0.8

    # Create empty gameboard. 0 is empty, 1 is x, 2 is 0
    gameBoard = np.zeros([3,3])

    # Grayscale threshold. 255 = white, 0 = black.
    # TODO: add interactive thresholding??? Maybe adaptive thresholding???
    gsThreshold = 200

    players = ['X', 'O']
    firstPlayer = 'O' # Robot plays as X

    def __init__(self, debugFlag=False):
        print("started")
        self.debugFlag = debugFlag
        self.gameAI = brainBox.GameAI()

    def PlayfieldCalibration(self, imageCapture):
        # Create calibration object
        cal = CalibratePlayfield()
        # return the region location and outerRadii.
        self.regionCenters, self.outerRadii = cal.FindPlayCircles(imageCapture)
        # return teh ROIs for each image
        self.regionROIs = cal.CreateROIs(self.regionCenters, self.outerRadii)
        # using the outerRadii, caluclate max inner radii for circle detect
        self.maxInnerRadii = np.uint16(np.around(self.outerRadii*
                                                 self.ratioInnerOuter))
    
    def ScanSquares(self,imageCapture):
        gsCapture = cv.cvtColor(imageCapture,cv.COLOR_BGR2GRAY)
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
                self.__UpdateGameboard__(iRegion,symbol)
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
        """
        currentRegionValue = self.gameAI.squares[index]
        if currentRegionValue == None:
            self.gameAI.addMarker(index,symbol)
            return
        if currentRegionValue == symbol:
            return
        else:
            print('CHEATER!!!!')
            return


if __name__ == "__main__":
    emptyPath = r"C:\Users\hartl\Google Drive\05 - ENS Work\05 - Robotic Arm Project\Test Tic Tac Toe Images\RealBlank.jpg"
    emptyImg = cv.imread(emptyPath)
    testImagePath = r"C:\Users\hartl\Google Drive\05 - ENS Work\05 - Robotic Arm Project\Test Tic Tac Toe Images\Real2O2X.jpg"
    testImagePath = cv.imread(testImagePath)
    cv.imshow('BaseImage',testImagePath)
    gameControl = TicTacToeControl(debugFlag=True)
    gameControl.PlayfieldCalibration(emptyImg)
    gameControl.ScanSquares(testImagePath)
    gameControl.gameAI.showBoard()

    cv.destroyWindow