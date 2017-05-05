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
	//number of image path skipped because of some error
	int skiped = 0;
	//load the background image
	Mat background = imread("C:/Users/hjalt/Google Drev/Uni/P4 - Project/project/Pictures/New pictures/Normal pipe/N-R-F.jpg", 1);
	if (!background.data){
			return -1;
	}
	// read the image paths from this file 
	ifstream input("./pathnew.txt");
	
	namedWindow("photo", CV_WINDOW_KEEPRATIO);
	// Go through each image in the path file.
	// The Path struct is used here so it's easier to extract the
	// path, filename and file extention when the images is saved.
	for (Path pathName; getline(input, pathName.completepath);) {
		// load the image from the pathname
		Mat img = imread(pathName.completepath, 1);

		if (!img.data) { // if the images is not loaded correctly, continue to the next one
			skiped++;
			continue;
		}

		/// find the root
		Mat result = AlgorithmRoots(background, img, pathName.filename());

		// show the result. 
		imshow("photo", result);
		cv::waitKey(1);

		/// save the result and save a log file at the same location. 
		saveimg(result, pathName, pathName);

		
	}

	// wait a little and then close
	cout << endl << "NUMBER OF IMAGES SKIPPED IN THE PATH FILE: " << skiped;
	cv::waitKey(0);
	 
	
	return 0;
}



