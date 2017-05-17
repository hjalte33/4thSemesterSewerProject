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

	struct Features {
		Features();
		Features(std::string _name, cv::Mat input, cv::Mat orgSrc);

		std::string name;
		long int area;
		long int arclength;
		float shapeVariance;
		float avgAngleChange;
		float perimAreaRatio;
		float boundBoxAspRatio;
		float distFromCenter;
		float avgColourOrigImg;

		void writeFeaturesToFile(const std::string & fiName, Path inputFiPath);
		void coutData();
		cv::Mat getFeaturAsMat();
	private:
		bool doesFiExists(const std::string & name);
		
	};

	Features FSFeaturs;
	Features ROFeaturs;
	Features RBFeaturs;

	cv::Mat featuresMat;

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

	void writeTrainingdata();

	void calculateFeatures();

	cv::Mat getFeatures();

	void calculateScores();


};

#endif

