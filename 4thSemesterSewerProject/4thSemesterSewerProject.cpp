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

	string FSTrainDataPath = "C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/FS.csv";
	string FSlabelsPath = "C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/FS_labels.csv";
	KnnData FSTrainData = KnnData(FSTrainDataPath, FSlabelsPath);

	string ROTrainDataPath = "C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/RO.csv";
	string ROlabelsPath = "C:/HB/cppSpace/4thSemesterSewerProject/4thSemesterSewerProject/features/RO_labels.csv";
	KnnData ROTrainData = KnnData(ROTrainDataPath, ROlabelsPath);


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

		ImgObj image = ImgObj(srcPathName, refPathName);

		if (!image.readImages()) {
			readError++;
			continue;
		}

		// here the magic happens
		// image.calculateTrainingdata();
		image.calculateFeatures();
		
		float FSScore = FSTrainData.findKnn(image.getFeatures().row(0));
		float ROScore = ROTrainData.findKnn(image.getFeatures().row(1));
		float RBScore = RBTrainData.findKnn(image.getFeatures().row(2));
		cout << "FS " << FSScore << endl;
		cout << "RO " << ROScore << endl;
		cout << "RB " << RBScore << endl << endl;
	}

	// wait a little and then close
	cout << endl << "I skiped " << skiped << " images, and hand trouble reading " << readError << " images";
	cv::waitKey(0);
	 
	
	return 0;
}



