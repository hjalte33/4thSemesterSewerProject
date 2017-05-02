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


//Find the root and returns an image with the result.
Mat findRoot(cv::Mat background, cv::Mat src) {
	//substract the background
	Mat result;
	absdiff(src, background, result);
	return result;
}

void findingContours(cv::Mat src) {
	vector<vector<Point>> contours;
	findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	 
	double area = 0;
	double lastArea = 0;

	Point lastPt1, lastPt2;
	lastPt1.x = 0;
	lastPt1.y = 0;
	lastPt2.x = 1;
	lastPt2.y = 1;

	for (int i = 0; i<contours.size(); i++) {
		Rect rect = boundingRect(contours[i]);
		Point pt1, pt2;
		pt1.x = rect.x;
		pt1.y = rect.y;
		pt2.x = rect.x + rect.width;
		pt2.y = rect.y + rect.height;

		area = contourArea(contours[i]);

		if (area > lastArea) {
			//rectangle(newImgOut, pt1, pt2, Scalar(155), 1);
			cout << "Area of crack " << i << ": " << area << endl;
			cout << "point: " << "(" << pt1.x << "," << pt1.y << ")" << endl;
			cout << "Last point: " << "(" << lastPt1.x << "," << lastPt1.y << ")" << endl << endl;

			for (int x = lastPt1.x; x < lastPt2.x + 1; x++) {
				for (int y = lastPt1.y; y < lastPt2.y + 1; y++) {
					src.at<uchar>(y, x) = 0;
				}
			}

			lastArea = area;
			lastPt1.x = rect.x;
			lastPt1.y = rect.y;

			////These two lines make this function not work
			//lastPt2.x = pt2.x;
			//lastPt2.y = pt2.y;

		}


		else {
			//rectangle(newImgOut, pt1, pt2, Scalar(0), 1);
			for (int x = pt1.x; x < pt2.x; x++) {
				for (int y = pt1.y; y < pt2.y; y++) {
					src.at<uchar>(y, x) = 0;
				}
			}
		}
	}

	for (int i = 0; i < contours.size(); i++) {
		drawContours(src, contours, i, Scalar(155), 2);
	}

}


cv::Mat findRoot2(cv::Mat background, cv::Mat src){

	int kernelSize = 30;
	Mat blurred;
	//blur the background image
	blur(background, blurred, Size(kernelSize, kernelSize));

	//substract the background
	src = blurred - src;

	int elementSize = 14;
	//Morphology to reduce noise in the picture
	Mat element = getStructuringElement(0, Size(elementSize, elementSize));
	morphologyEx(src, src, MORPH_OPEN, element);
	morphologyEx(src, src, MORPH_CLOSE, element);

	cvtColor(src, src, CV_RGB2GRAY);
	threshold(src, src, 30, 255, THRESH_TRIANGLE);

	findingContours(src);
	return src;
}