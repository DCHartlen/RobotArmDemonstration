import numpy as np
import cv2 as cv
from CalibratePlayfield import CalibratePlayfield

class TicTacToeControl:
    # Define ratio of inner circle (gamepeice) to outer circle (playfield)
    ratioInnerOuter = 0.8

    # Create empty gameboard. 0 is empty, 1 is x, 2 is 0
    gameBoard = np.zeros([3,3])

    # Grayscale threshold. 255 = white, 0 = black.
    # TODO: add interactive thresholding??? Maybe adaptive thresholding???
    gsThreshold = 200

    def __init__(self):
        print("started")

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

            # scan through each region looking for circles
            gamePiece = cv.HoughCircles(currentROI,cv.HOUGH_GRADIENT, 1, 2,
                                        maxRadius=self.maxInnerRadii[0])
            
            # If a cirlce is found, determine if it is an X or 0
            if gamePiece is not None:
                tokenCenter= np.uint16(np.around(gamePiece[0,0,0:2]))
                symbol = self.DetectSymbol(currentROI, tokenCenter)
                # place symbol in the gameboard
                symbol = self.__UpdateGameboard(iRegion,symbol)

            # If there is no piece in the square, print message
            # TODO: add logic to detect hands, say the loss of a circle
            # else:   
                # print("empty")
        print(self.gameBoard)

    def DetectSymbol(self, currentROI, tokenCenter):
        # Get the average of a [6,6] pixel array at the center of the gamepiece
        subset = currentROI[(tokenCenter[0]-3):(tokenCenter[0]+3),
                            (tokenCenter[1]-3):(tokenCenter[1]+3)]
        # Compute the overall mean value of the pixels. 
        subsetAvg = np.mean(np.mean(subset))
        # Base on average pixel value, determine if X or O. X has white center,
        # O has a grey center
        if subsetAvg > self.gsThreshold:
            # print('FOUND: X!!!')
            return 1
        else:
            # print('FOUND: O!!!')
            return 2

    def __GetImageROI__(self,imageCapture,region):
        roi = imageCapture[region[0]:region[1], region[2]:region[3]]
        return roi

    def __UpdateGameboard(self,linearIndex,symbol):
        # Need to generate xy coordinates from a single integer.
        # Private function prevents access from outside the class
        # First, floor divide to get the y-index.
        y = linearIndex//3
        # Now, get x by subtracting y*3 from linear index
        x = linearIndex-(y*3)
        # Now append gameboard
        self.gameBoard[x,y] = symbol


if __name__ == "__main__":
    emptyPath = r"C:\Users\hartl\Documents\GitHub\RobotArmDemonstration\PythonSourceCode\TestImages\EmptyBoardStraight.jpg"
    emptyImg = cv.imread(emptyPath)
    twoPiecePath = r"C:\Users\hartl\Documents\GitHub\RobotArmDemonstration\PythonSourceCode\TestImages\Board1x1o.jpg"
    twoPieceImage = cv.imread(twoPiecePath)
    cv.imshow('BaseImage',twoPieceImage)
    gameControl = TicTacToeControl()
    gameControl.PlayfieldCalibration(emptyImg)
    gameControl.ScanSquares(twoPieceImage)