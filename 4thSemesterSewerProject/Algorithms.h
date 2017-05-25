/**
This is a low level general algoriths library. 
it contains functions for segmentation and algoriths to extract individual features from a binary image

**/
#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "opencv2\core.hpp"

using namespace cv;
using namespace std;

void translateImg(Mat img, int offsetx, int offsety);

void showImages(Mat image1, Mat image2, Mat image3, Size size, int waitkey);

long int CountPixelBrightness(Mat inputImage);

long int CountWhitePixels(Mat inputImage);

Mat rgbToGray(Mat src, float red, float green, float blue);

Mat rgbToGray(Mat src, int channel);

int getThresh(Mat inputImage);

Mat keepBlobBiggestArea(Mat inputImage);

Mat keepBlobBiggestEnclosingCircle(Mat inputImage);

// ------------------------------------------------------------------------
// ------------------- Segmentation functions -----------------------------
// ------------------------------------------------------------------------

/* Segmentation algorithm that is good for roots
*It uses the blue channel that has the higest brighness value for the roots 
* and everything else is relativly black. 
*/
Mat ROSegmentation(Mat src, Mat refImage);

/** Segmentation algorithm that is good for offset pipes
* It finds contours and delets the smallest ones.
* It uses the biggest bounding circle to determine the biggest contour.
**/
Mat FSSegmentation(Mat src, Mat refImage);

/** Segmentation algorithm that is good for cracks and holes
* It finds contours and delets the smallest ones. 
* It uses the area to detemine which contour is the lagest one. 
**/
Mat RBSegmentation(Mat src, Mat refImage);

// ------------------------------------------------------------------------
// -------------------- Classification functions --------------------------
// ------------------------------------------------------------------------

long int getAreaFeature(Mat inputImage);

long int getArclengthFeature(Mat inputImage);

float getShapeVariance(Mat inputImage, int pd);

float getAverageAngleChange(Mat inputImage, int pd);

float getPerimeterToAreaRatio(Mat inputImage);

float getBoundBoxAspectRatio(Mat inputImage);

float getDistToCenter(Mat inputImage);

float getAverageColourIntensity(Mat inputImage, Mat originalImage, int Channel);


#endif