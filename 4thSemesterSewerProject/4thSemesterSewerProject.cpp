// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace cv;
using namespace std;

void translateImg(Mat img, int offsetx, int offsety) {
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
}

void centering(Mat src, int minRadius, int step = 2) {

	Mat blured;
	blur(src, blured, Size(9, 9));
	vector<Vec3f> circles;

	for (int i = minRadius; circles.size() < 1 && i < blured.rows / 2; i += step) {
		HoughCircles(blured, circles, CV_HOUGH_GRADIENT, 1, blured.rows / 6, 10, 100, minRadius, i);
	}

	int shiftx = circles[0][0] - src.cols / 2;
	int shifty = circles[0][1] - src.cols / 2;

	cout << shiftx << "  " << shifty << endl;

	translateImg(src,shiftx,shifty);

}

int main()
{
	Mat img = imread("./data/hole1.JPG", 0);
	Mat background = imread("./data/background.JPG", 0);
	Mat diff = Mat(img.rows, img.cols, 0);

	if (!img.data)
	{
		return -1;
	}

	

	// find the center of the image
	centering(img, 100,5);
	

	/// Draw the circles detected
	

	/// Show your results
	
	namedWindow("Hough Circle Transform Demo",CV_WINDOW_KEEPRATIO);
	imshow("Hough Circle Transform Demo", img);

	//blur(background, background, Size(99, 99));
	absdiff(img, background, diff);

	namedWindow("photo", CV_WINDOW_KEEPRATIO);
	imshow("photo", background);
	waitKey(0);
	 
	
	return 0;
}



// //draw the cirkle
//for (size_t i = 0; i < circles.size(); i++)
//{
//	Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//	int radius = cvRound(circles[i][2]);
//	// circle center
//	circle(src, center, 3, Scalar(0, 255, 0), -1, 8, 0);
//	// circle outline
//	circle(src, center, radius, Scalar(0, 0, 255), 3, 8, 0);
//}

