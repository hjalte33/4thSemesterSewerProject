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
	// translate an image with offset x and y 
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
}

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
	translateImg(src,shiftx,shifty);
	//cout << shiftx << "  " << shifty << endl;   //debugging

	

}

int main()
{
	Mat img = imread("./data/root1.JPG", 0);
	Mat background = imread("./data/background.JPG", 0);
	Mat diff = Mat(img.rows, img.cols, 0);

	if (!img.data)
	{
		return -1;
	}

	

	// find the center of the image
	centering(img, 100,5);
	centering(background, 100, 5);

	//substract the background
	absdiff(img, background, diff);

	// show the result. 
	namedWindow("photo", CV_WINDOW_KEEPRATIO);
	imshow("photo", diff);
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

