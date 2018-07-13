import numpy as np
import cv2 as cv

# TODO: add doc strings to each function
class CalibratePlayfield:
    """
    Class handles all calibration of the playing field, including finding
    regions, calculating ROI
    """
    ratioRadiiSquare = 1.1 # Ratio of circle diameter to game square

    def __init__(self,debugFlag=False):
        self.debugFlag = debugFlag
        if debugFlag == True:
            print("Debug Mode Activated")

    def FindPlayCircles(self,imageCapture):
        workingImage = imageCapture.copy()
        self.imageDims = workingImage.shape
        self.imageDims = self.imageDims[0:2] # Remove # of channels

        # convert copy to greyscale for circle algorithm
        gsImage = cv.cvtColor(workingImage, cv.COLOR_BGR2GRAY)

        # Find all clrcles on the playing field.
        # set max circle radius to be 1/6 the smallest image dim
        maxCircleRadius = int(min(self.imageDims)/6)
        # circles must be seperated by atleast 2 maxCircRadius
        minCircleSep = int(maxCircleRadius)
        # Run Hough Circle detection algorithm
        circles = cv.HoughCircles(gsImage,
                                  cv.HOUGH_GRADIENT,
                                  2,
                                  minCircleSep,
                                  maxRadius = maxCircleRadius,)

        # round dimensions and radii found by detection to ints
        circles = np.uint16(np.around(circles))
        circles = circles[0]

         # sort circles, assuming that camera is to the left of image. Therefore:
        #    0 | 1 | 2 
        #    3 | 4 | 5
        #    6 | 7 | 8
        # Start by sorting circles left to right
        sortIndex = np.argsort(circles[:, 1], axis=0)
        circles = circles[sortIndex, :]
        # Now order top down. Must be done column by column.
        for i in [0, 3, 6]:
            sortIndex = np.argsort(circles[i:i+3, 0], axis=0)
            circles[i:i+3, :] = circles[sortIndex+i, :]

        # TODO: make sure cicle ordering is consistant.

        # If in debug mode, make an image of the circles found
        if(self.debugFlag == True):
            j = 0
            for i in circles:
                # Print outer radius
                cv.circle(workingImage, (i[0], i[1]), i[2], (0, 255, 0), 2)
                # Print centers
                cv.circle(workingImage, (i[0], i[1]), 2, (0, 0, 255), 3)
                # Add text
                font = cv.FONT_HERSHEY_SIMPLEX
                cv.putText(workingImage, str(j), (i[0], i[1]), font,
                           2, (255, 0, 0), 2, cv.LINE_AA)
                j+=1
                cv.imshow('Circles Detected', workingImage)
            cv.waitKey(0)

        circleCenters = circles[:, 0:2]
        circleRadii = circles[:,2]

        return circleCenters, circleRadii

    def CreateROIs(self, circleCenters, circleRadii):
        # Regions are [xMin,xMax,yMin,yMax]
        gameRegions = np.zeros([9,4])

        # For all nine circles, create an ROI
        for i in range(len(gameRegions)):
            gameRegions[i,:] = [circleCenters[i,0]-self.ratioRadiiSquare*circleRadii[i],
                circleCenters[i,0]+self.ratioRadiiSquare*circleRadii[i],
                circleCenters[i,1]-self.ratioRadiiSquare*circleRadii[i],
                circleCenters[i,1]+self.ratioRadiiSquare*circleRadii[i]]
        
        # ROIs must be integers
        gameRegions = np.uint16(np.around(gameRegions))
        # Return the ROIs
        return gameRegions

    def GetImageDims(self):
        return self.imageDims
        


if __name__ == "__main__":
    path = r"C:\Users\hartl\Documents\GitHub\RobotArmDemonstration\PythonSourceCode\TestImages\EmptyBoardStraight.jpg"
    print(path)

    baseImage = cv.imread(path)
    testCal = CalibratePlayfield(True)
    centers, radii = testCal.FindPlayCircles(baseImage)
    regions = testCal.CreateROIs(centers,radii)
    print(regions[0])
    
    for i in regions:
        cv.imshow('Region', baseImage[i[0]:i[1], i[2]:i[3]])
        cv.waitKey(0)
        cv.destroyWindow

    print(testCal.imageDims)
    