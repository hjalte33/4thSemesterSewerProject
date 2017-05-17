// 4thSemesterSewerProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Path.h"
#include "ImgObj.h"
#include "KnnData.h"


using namespace cv;
using namespace std;
using namespace cv::ml;



int main()
{
	string RBTrainDataPath = "C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/RB.csv";
	string RBlabelsPath = "C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/RB_labels.csv";
	KnnData RBTrainData = KnnData(RBTrainDataPath, RBlabelsPath);


	//number of image path skipped because of some error
	int skiped = 0;
	int readError = 0;
	// read the image paths from this file 
	ifstream inputImgPath("./features/trainingDataPaths.txt");
	
	// load the background image 
	Path refPathName = Path("C:/Users/hjalt/Google Drev/Uni/P4 - Project/project/Pictures/New pictures/Normal pipe/Mean.png");
	
	// Go through each image in the path file.
	// The Path struct is used here so it's easier to extract the
	// path, filename and file extention when the images is saved.
	for (Path srcPathName; getline(inputImgPath, srcPathName.completepath);) {
		
		int outcomented = srcPathName.completepath.find_first_of("#");
		if (outcomented != -1) {
			skiped++;
			continue;
		}
		ImgObj image = ImgObj(srcPathName, refPathName, RBTrainData.dist);
		if (!image.readImages()) {
			readError++;
			continue;
		}

		// here the magic happens
		image.calculateTrainingdata();
		image.calculateScores(RBTrainData.dist);
	}

	// wait a little and then close
	cout << endl << "I skiped " << skiped << " images, and hand trouble reading " << readError << " images";
	cv::waitKey(0);
	 
	
	return 0;
}



