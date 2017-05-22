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
	string testno = "9";
	string testFolderName = "C:/Users/hjalt/Desktop/test" + testno + "/";
	
	// reading the training data and train knn
	string RBTrainDataPath = testFolderName + "RB.csv";
	string RBlabelsPath = testFolderName + "RB_labels.csv";
	KnnData RBTrainData = KnnData(RBTrainDataPath, RBlabelsPath);

	string FSTrainDataPath = testFolderName + "FS.csv";
	string FSlabelsPath = testFolderName + "FS_labels.csv";
	KnnData FSTrainData = KnnData(FSTrainDataPath, FSlabelsPath);

	string ROTrainDataPath = testFolderName + "RO.csv";
	string ROlabelsPath = testFolderName + "RO_labels.csv";
	KnnData ROTrainData = KnnData(ROTrainDataPath, ROlabelsPath);


	//number of image path skipped because of some error
	int skiped = 0;
	int readError = 0;
	// read the image paths from this file 
	//ifstream inputImgPath("C:/Users/hjalt/Desktop/test1/trainingPhotosPaths.txt");
	ifstream inputImgPath(testFolderName + "testPhotosPaths (" + testno + ").txt");


	// load the background image 
	Path refPathName = Path("C:/Users/hjalt/Desktop/testMaster/Mean.png");
	
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
		//image.writeTrainingdata();
		image.calculateFeatures();
		
		float FSScore = FSTrainData.findKnn(image.getFeaturesMat().row(0));
		float ROScore = ROTrainData.findKnn(image.getFeaturesMat().row(1));
		float RBScore = RBTrainData.findKnn(image.getFeaturesMat().row(2));
		cout << "the imagename is: " << image.srcPath.filename().substr(0,2) << ",";
		cout << "FS score ," << FSScore << ",";
		cout << "RO score ," << ROScore << ",";
		cout << "RB score ," << RBScore << "," << endl;
		
		ofstream writeScores(testFolderName + "scores.csv", std::ios_base::app);
		writeScores << std::fixed;
		writeScores << image.srcPath.filename().substr(0, 2) << "\t";
		writeScores << FSScore << "\t";
		writeScores << ROScore << "\t";
		writeScores << RBScore << "\t" << endl;
	}

	// wait a little and then close
	cout << endl << "I skiped " << skiped << " images, and hand trouble reading " << readError << " images";
	cv::waitKey(0);
	 
	
	return 0;
}



