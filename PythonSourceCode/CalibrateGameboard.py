"""
CalibrateGameboard.py

Created by: D.C. Hartlen
Date:       19-Aug-2018
Updated by: 
Date:

File consists of a single class "CalibrateGameboard". Provided an image array
of an empty gameboard, finds the center of each square, and returns the center
of each square and the size of the square.

Note: must use specially designed gameboard which has circles inscribed within
each square. Finding circles is substancially easier than finding the center
of multiple squares.
"""

import numpy as np
import cv2 as cv

class CalibrateGameboard:
    """
    Class handles calibration of the playfield. Constructor takes one argument,
    a debug flag.
    Inputs: 
        debugFlag (boolean): defaults to False. Setting true enables terminal 
            printing. Not useful if class is used within another function.
    """
    ratioRadiiSquare = 1.1 # Ratio of circle diameter to game square

    def __init__(self,debugFlag=False):
        self.debugFlag = debugFlag
        if debugFlag == True:
            print("Debug Mode Activated")

    def FindPlayCircles(self,imageCapture):
        """
        Finds the center of each square in the gamboard. Really, it finds the
        center of each inscribed circles. Also returns size of inscribed circle
        Inputs:
            imageCapture: BGR image capture.
        Returns:
            circleCenters: Nine entry list of circle centers.
            circleRadii: Nine entry list of circle radii
        """
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
        """
        Given the set of circle centers and radii, generate a region of
        interest for each square on the gameboard. This makes circle detection
        quicker later on. Probably. Definately makes the logic easier. 
        Inputs:
            circleCenters: Nine entry list of circle centers.
            circleRadii: Nine entry list of circle radii
        Returns:
            gameRegions: 9x4 np array of ROIS. Regions are listed by
                [yMin, yMax, xMin, xMax].
        """
        # Regions are [xMin,xMax,yMin,yMax]
        gameRegions = np.zeros([9,4])

        # For all nine circles, create an ROI
        for i,radii in enumerate(circleRadii):
            # Regions are collected [y,x] as images indexed [rows, columns]
            gameRegions[i,:] = [circleCenters[i,1]-self.ratioRadiiSquare*radii,
                circleCenters[i,1]+self.ratioRadiiSquare*radii,
                circleCenters[i,0]-self.ratioRadiiSquare*radii,
                circleCenters[i,0]+self.ratioRadiiSquare*radii]
        
        # ROIs must be integers
        gameRegions = np.uint16(np.around(gameRegions))
        # Return the ROIs
        return gameRegions

    def GetImageDims(self):
        """
        Returns the image dimensions
        """
        return self.imageDims
        

""" 
Standalone execution loads an image from disk and runs calibration.
For testing purposes only.
"""
if __name__ == "__main__":
    path = r".\testImages\RealBlank.jpg"
    print(path)

    baseImage = cv.imread(path)
    testCal = CalibrateGameboard(True)
    centers, radii = testCal.FindPlayCircles(baseImage)
    regions = testCal.CreateROIs(centers,radii)
    
    for i,region in enumerate(regions):
        print(region)
        temp = baseImage.copy()
        temp = baseImage[region[0]:region[1], region[2]:region[3]]
        cv.imshow('Region', temp)
        cv.waitKey(0)
        cv.destroyWindow

    print(testCal.imageDims)
    