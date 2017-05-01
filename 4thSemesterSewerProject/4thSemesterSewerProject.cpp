// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
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


struct Path {/* Struct to take care of deviding the path
			 into directory, filename and extention */
	//The complete path. Works just like a nomal string. 
	std::string completepath;
	std::string dir();
	std::string filename();
	std::string ext();
	Path();
	Path(const std::string _path); 
};


//returns the directory of the path
std::string Path::dir() { 
	std::size_t foundslash = completepath.find_last_of("/\\");
	std::size_t founddot = completepath.find_last_of(".");
	return completepath.substr(0, foundslash + 1);
}

//returns the filename (if any) of the path
std::string Path::filename() {
	std::size_t foundslash = completepath.find_last_of("/\\");
	std::size_t founddot = completepath.find_last_of(".");
	return completepath.substr(foundslash + 1, founddot-foundslash-1);
}

// returns the file extention (if any) of the path
std::string Path::ext() {
	std::size_t foundslash = completepath.find_last_of("/\\");
	std::size_t founddot = completepath.find_last_of(".");
	return completepath.substr(founddot);
}

// default constructor
Path::Path(){
}

/*constructor for the path struct
takes one argument which is the full path as a string */
Path::Path(const std::string _path){
	completepath = _path;
}

// Saves the image at the same location and write the path to the outfile
void saveimg(ofstream &writeoutfile, cv::Mat img, Path path, std::string suffix = "_result") { 
	
	string savepathname = path.dir() + path.filename() + suffix + path.ext();
	imwrite(savepathname, img);
	writeoutfile  << savepathname << endl;
}

// Saves the image at the same location 
void saveimg(cv::Mat img, Path path, std::string suffix = "_result") { 
	
	string savepathname = path.dir() + path.filename() + suffix + path.ext();
	imwrite(savepathname, img);
}


int main()
{
	//load the background image
	Mat background = imread("C:\\Users\\hjalt\\Google Drev\\Uni\\P4 - Project\\project\\Pictures\\NormalPipeWhiteFull.png", 0);
	if (!background.data)
		{
			return -1;
		}
	// read the image paths from this file 
	ifstream input("./paths.txt");
	//string astring;
	//getline(input, astring);
	// save the output paths in this text file
	ofstream writeoutfile("SavedImgPaths.txt");

	// Go through each image in the path file.
	// The Path struct is used here so it's easier to extract the
	// path, filename and file extention when the images is saved.
	for (Path pathName; getline(input, pathName.completepath);) {
		
		Mat img = imread(pathName.completepath, 0);
		Mat diff = Mat(img.rows, img.cols, 0);

		if (!img.data)
		{
			return -2;
		}

		// find the center of the image
		//centering(img, 30,5);
		//centering(background, 30, 5);

		//substract the background
		absdiff(img, background, diff);

		// show the result. 
		namedWindow("photo", CV_WINDOW_KEEPRATIO);
		imshow("photo", diff);
		cv::waitKey(1);


		saveimg(writeoutfile, diff, pathName);

	}

	// wait a little and then close
	cv::waitKey(2000);
	 
	
	return 0;
}



