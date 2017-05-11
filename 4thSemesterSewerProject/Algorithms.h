#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "opencv2\core.hpp"

using namespace cv;
using namespace std;

void translateImg(Mat img, int offsetx, int offsety);

long int CountPixelBrightness(Mat inputImage);

long int CountWhitePixels(Mat inputImage);

Mat rgbToGray(Mat src, float red, float green, float blue);

Mat rgbToGray(Mat src, int channel);

Mat subtractRef(Mat inputImage, Mat ref);

int getThresh(Mat inputImage);

Mat areaThresh(Mat inputImage);

void showImages(Mat image1, Mat image2, Mat image3, Size size, int waitkey);


// ------------------------------------------------------------------------
// ------------------- Segmentation functions -----------------------------
// ------------------------------------------------------------------------

Mat ROESegmentation(Mat src, Mat refImage);

Mat FSSegmentation(Mat src, Mat refImage);

Mat RBSegmentation(Mat src, Mat refImage);




// ------------------------------------------------------------------------
// -------------------- Classification functions --------------------------
// ------------------------------------------------------------------------

long int getAreaFeature(Mat inputImage);

long int getArclengthFeature(Mat inputImage);

double getShapeVariance(Mat inputImage, int pd);

double AverageAngleChange(Mat inputImage, int pd);

double PerimeterToAreaRatio(Mat inputImage);

double boundBoxAspectRatio(Mat inputImage);

float getDistToCenter(Mat inputImage);

float AverageColourIntensity(Mat inputImage, Mat originalImage, int Channel);


#endif