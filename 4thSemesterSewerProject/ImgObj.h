#ifndef IMGOBJ_H
#define IMGOBJ_H

#include "Path.h"
#include "opencv2\core.hpp"

using namespace cv;
using namespace std;

class ImgObj {
private:
	Mat src;
	Mat ref;
	bool isImagesLoaded = false;
	int imgclass;
	cv::Mat featuresMat;
	
	struct Features {
		Features(std::string _name, cv::Mat input, cv::Mat orgSrc);
		cv::Mat getFeaturAsMat();
		void writeFeaturesToFile(const std::string & fiName, Path inputFiPath);
		void coutData();
	private:
		std::string name;
		long int area;
		long int arclength;
		float shapeVariance;
		float avgAngleChange;
		float perimAreaRatio;
		float boundBoxAspRatio;
		float distFromCenter;
		float avgColourOrigImg;		
		bool doesFileExist(const std::string & name);
	};

public:
	// the constructor calculates all the features.
	ImgObj(Path _srcPath, Path _refPath);

	Path srcPath = Path();
	Path refPath = Path();

	bool  readImages();
	Mat getSrcFromFile();
	Mat getRefFromFile();

	void saveimg(std::string suffix);
	void saveimg(cv::Mat img, Path imgsavepath, std::string suffix);
	void saveimg(cv::Mat img, Path imgsavepath, Path logsavepath, std::string suffix);

	//This functions calculates the features and write the nessesary files with the trainingdata
	void writeTrainingdata();

	//calculates the features and pushes them the the featuresMatrix 
	void calculateFeatures();

	//returs the featursMatrix
	cv::Mat getFeaturesMat();

};

#endif

