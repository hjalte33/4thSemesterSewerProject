#pragma once

#include "stdafx.h"
#include "Path.h"

// Translate an image in x and y
void translateImg(cv::Mat img, int offsetx, int offsety);

/* Tries to find the smallest cirkle bigger than minRadius
and then center the image at that */
void centering(cv::Mat src, int minRadius, int step = 2);


void findingContours(cv::Mat src);

// not yet implemented
cv::Mat findRoot(cv::Mat background, cv::Mat src);

cv::Mat rgbToGray(cv::Mat src);

// Christoffers attempt
cv::Mat findRoot2(cv::Mat background, cv::Mat src);

cv::Mat AlgorithmRoots(Mat backgroundImage, Mat inputImage);

cv::Mat findRoot3(cv::Mat & background, cv::Mat & src);

cv::Mat findRoot4(cv::Mat background, cv::Mat src);

