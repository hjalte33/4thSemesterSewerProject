// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Path.h"
#include "ImgObj.h"


using namespace cv;
using namespace std;

int main()
{
	//number of image path skipped because of some error
	int skiped = 0;

	// read the image paths from this file 
	ifstream input("./pathnew.txt");
	
	//namedWindow("photo", CV_WINDOW_KEEPRATIO);
	
	Path refPathName = Path("C:/Users/hjalt/Google Drev/Uni/P4 - Project/project/Pictures/New pictures/Normal pipe/N-W-F.jpg");
	
	// Go through each image in the path file.
	// The Path struct is used here so it's easier to extract the
	// path, filename and file extention when the images is saved.
	for (Path srcPathName; getline(input, srcPathName.completepath);) {
		int outcomented = srcPathName.completepath.find_first_of("#");
		if (outcomented != -1) continue;
		ImgObj image = ImgObj(srcPathName, refPathName);
		if (!image.readImages()) {
			continue;
		}

		image.calculateScores();
	
	}

	// wait a little and then close
	cout << endl << "I skiped " << skiped << " images";
	cv::waitKey(0);
	 
	
	return 0;
}



