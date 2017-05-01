// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Algorithms.h"
#include "Path.h"


using namespace cv;
using namespace std;


// Saves the image at the same location 
void saveimg(cv::Mat img, Path imgsavepath, std::string suffix = "_result") {

	string savepathname = imgsavepath.dir() + imgsavepath.filename() + suffix + imgsavepath.ext();
	imwrite(savepathname, img);
}

// Saves the image at the same location and write the path to the outfile
void saveimg(cv::Mat img, Path imgsavepath, Path logsavepath, std::string suffix = "_result") {

	string savepathname = imgsavepath.dir() + imgsavepath.filename() + suffix + imgsavepath.ext();
	imwrite(savepathname, img);
	ofstream writeoutfile(logsavepath.dir()+"saved log.txt", std::ios_base::app);
	writeoutfile << savepathname << endl;
}


int main()
{
	//load the background image
	Mat background = imread("C:\\Users\\hjalt\\Google Drev\\Uni\\P4 - Project\\project\\Pictures\\NormalPipeWhiteFull.png", 0);
	if (!background.data){
			return -1;
		}
	// read the image paths from this file 
	ifstream input("./paths.txt");

	// Go through each image in the path file.
	// The Path struct is used here so it's easier to extract the
	// path, filename and file extention when the images is saved.
	for (Path pathName; getline(input, pathName.completepath);) {
		// load the image from the pathname
		Mat img = imread(pathName.completepath, 0);
		
		Mat diff = Mat(img.rows, img.cols, 0);
		if (!img.data){
			return -2;
		}

		/// find the root
		findRoot(img);

		//substract the background
		absdiff(img, background, diff);

		// show the result. 
		namedWindow("photo", CV_WINDOW_KEEPRATIO);
		imshow("photo", diff);
		cv::waitKey(1);

		/// save the result and save a log file at the same location. 
		saveimg(diff, pathName, pathName);
		
	}

	// wait a little and then close
	cv::waitKey(2000);
	 
	
	return 0;
}



