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

	struct Scores {
		float RB = -1;
		float FS = -1;
		float ROE = -1;
	}scores;

	struct Features {
		Features(std::string _name, cv::Mat input, cv::Mat orgSrc);
		
		std::string name;
		long int area;
		long int arclength;
		float shapeVariance;
		float avgAngleChange;
		float PerimAreaRatio;
		float BoundBoxAspRatio;
		float distFromCenter;
		float AvgColourOrigImg;

		void writeFeaturesToFile(const std::string & fiName);
		void coutData();
	};

public:
	ImgObj(Path _srcPath, Path _refPath);

	Path srcPath = Path();
	Path refPath = Path();

	//Scores getScores();

	bool  readImages();
	Mat getSrcFromFile();
	Mat getRefFromFile();

	void saveimg(std::string suffix);
	void saveimg(cv::Mat img, Path imgsavepath, std::string suffix);
	void saveimg(cv::Mat img, Path imgsavepath, Path logsavepath, std::string suffix);

	void calculateScores();


};

#endif

