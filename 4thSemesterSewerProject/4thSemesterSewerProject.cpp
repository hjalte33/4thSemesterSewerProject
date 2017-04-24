// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main()
{
	Mat img = imread("./data/photo.png", 0);
	imshow("photo", img);
	waitKey(0);
	
	
	return 0;
}

