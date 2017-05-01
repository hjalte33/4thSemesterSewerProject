#pragma once

#include "stdafx.h"
#include "Path.h"

// Translate an image in x and y
void translateImg(cv::Mat img, int offsetx, int offsety);

/* Tries to find the smallest cirkle bigger than minRadius
and then center the image at that */
void centering(cv::Mat src, int minRadius, int step = 2);

// not yet implemented
void findRoot(cv::Mat src);