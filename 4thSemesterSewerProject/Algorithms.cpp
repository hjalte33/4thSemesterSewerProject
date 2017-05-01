#include "Algorithms.h"
#include "stdafx.h"

using namespace cv;
using namespace std;



// Translate an image in x and y
void translateImg(Mat img, int offsetx, int offsety) {
	// translate an image with offset x and y 
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
}

/* Tries to find the smallest cirkle bigger than minRadius
and then center the image at that */
void centering(Mat src, int minRadius, int step = 2) {
	//first create a blured image to reduce some noice
	Mat blured;
	blur(src, blured, Size(9, 9));
	//vector to store the circles found
	vector<Vec3f> circles;

	//inrease the circle diametre untill a circle is found. 
	for (int i = minRadius; circles.size() < 1 && i < blured.rows / 2; i += step) {
		HoughCircles(blured, circles, CV_HOUGH_GRADIENT, 1, blured.rows / 6, 10, 100, minRadius, i);
	}

	//translate the image so it's centered
	int shiftx = circles[0][0] - src.cols / 2;
	int shifty = circles[0][1] - src.cols / 2;
	translateImg(src, shiftx, shifty);
	//cout << shiftx << "  " << shifty << endl;   //debugging
}
