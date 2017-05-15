// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Path.h"
#include "ImgObj.h"


using namespace cv;
using namespace std;

int main()
{
	// read the training data from this file
	ifstream trainingDataPath("C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/RB.csv");
	Mat trainingData;
	for (string line; getline(trainingDataPath, line);) {
		stringstream ss(line);

		Mat row = Mat(1, 8, 0);
		string substr;
		for (int i = 0; getline(ss, substr, ','); i++) {
			row.at<uchar>(0, i) = stof(substr);
		}
		trainingData.push_back(row);
	}
	
	//number of image path skipped because of some error
	int skiped = 0;

	// read the image paths from this file 
	ifstream inputImgPath("./features/trainingDataPaths.txt");
	
	

	// load the background image 
	Path refPathName = Path("C:/Users/hjalt/Google Drev/Uni/P4 - Project/project/Pictures/New pictures/Normal pipe/Mean.png");
	
	// Go through each image in the path file.
	// The Path struct is used here so it's easier to extract the
	// path, filename and file extention when the images is saved.
	for (Path srcPathName; getline(inputImgPath, srcPathName.completepath);) {
		
		int outcomented = srcPathName.completepath.find_first_of("#");
		if (outcomented != -1) continue;
		
		ImgObj image = ImgObj(srcPathName, refPathName, trainingData);
		if (!image.readImages()) {
			continue;
		}

		// here the magic happens
		image.calculateTrainingdata();
	
	}

	// wait a little and then close
	cout << endl << "I skiped " << skiped << " images";
	cv::waitKey(0);
	 
	
	return 0;
}



