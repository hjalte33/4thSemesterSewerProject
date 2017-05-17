#include "stdafx.h"
#include "ImgObj.h"
#include "Algorithms.h"
#include "Path.h"


bool ImgObj::readImages() {
	src = cv::imread(srcPath.completepath);
	ref = cv::imread(refPath.completepath);
	if (!src.data || !ref.data) {
		return false;
	}
	else {
		isImagesLoaded = true;
		return true;
	}
}


ImgObj::ImgObj(Path _srcPath, Path _refPath) {
	srcPath = _srcPath;
	refPath = _refPath;
}

// Saves the source image at the same location 
void ImgObj::saveimg(std::string suffix = "_result") {
	string savepathname = srcPath.dir() + srcPath.filename() + suffix + srcPath.ext();
	imwrite(savepathname, src);
}


// Saves the specified image at the specified location 
void ImgObj::saveimg(cv::Mat img, Path imgsavepath, std::string suffix = "_result") {
	string savepathname = imgsavepath.dir() + imgsavepath.filename() + suffix + imgsavepath.ext();
	imwrite(savepathname, img);
}

// Saves the image at the same location and write the path to the outfile
void ImgObj::saveimg(cv::Mat img, Path imgsavepath, Path logsavepath, std::string suffix = "_result") {
	string savepathname = imgsavepath.dir() + imgsavepath.filename() + suffix + imgsavepath.ext();
	imwrite(savepathname, img);
	ofstream writeoutfile(logsavepath.dir() + "saved log.txt", std::ios_base::app);
	writeoutfile << savepathname << endl;
}


inline Mat ImgObj::getSrcFromFile(){
	return imread(srcPath.completepath);
}

inline Mat ImgObj::getRefFromFile(){
	return imread(refPath.completepath);
}

void ImgObj::writeTrainingdata() {
	// make sure that the images are loaded
	if (!isImagesLoaded) {
		if (!readImages()) return;
	}
	namedWindow("working on", WINDOW_KEEPRATIO);
	imshow("working on", src);
	waitKey(1);

	//create a features struct for each of the 3 segmentations 
	FSFeaturs = Features("FS", FSSegmentation(src, ref), src);
	ROFeaturs = Features("ROE", ROESegmentation(src, ref), src);
	RBFeaturs = Features("RB", RBSegmentation(src, ref), src);

	// accquire training data
	FSFeaturs.writeFeaturesToFile("FS", srcPath.completepath);
	ROFeaturs.writeFeaturesToFile("ROE", srcPath.completepath);
	RBFeaturs.writeFeaturesToFile("RB", srcPath.completepath);

	//cout for debugging
	cout << "Image name " << srcPath.filename() << endl;
	ROFeaturs.coutData();
	RBFeaturs.coutData();
	FSFeaturs.coutData();
}

void ImgObj::calculateFeatures() {
	// make sure that the images are loaded correctly
	if (!isImagesLoaded) {
		if (!readImages()) return;
	}
	namedWindow("working on", WINDOW_KEEPRATIO);
	imshow("working on", src);
	waitKey(1);

	//create a features struct for each of the 3 segmentations 
	FSFeaturs = Features("FS", FSSegmentation(src, ref), src);
	ROFeaturs = Features("RO", ROESegmentation(src, ref), src);
	RBFeaturs = Features("RB", RBSegmentation(src, ref), src);

	featuresMat.push_back(FSFeaturs.getFeaturAsMat());
	featuresMat.push_back(ROFeaturs.getFeaturAsMat());
	featuresMat.push_back(RBFeaturs.getFeaturAsMat());
}

cv::Mat ImgObj::getFeatures()
{
	return featuresMat;
}



void ImgObj::calculateScores() {

}




//----------------------------------------------------------------------------
//---------------------- Feature struct fuctions/constructors ----------------
//----------------------------------------------------------------------------

// constructor calculates all the fetures and populates the struct 
ImgObj::Features::Features() {

}

ImgObj::Features::Features(std::string _name, cv::Mat input, cv::Mat orgSrc) {
	name = _name;
	area = getAreaFeature(input);
	arclength = getArclengthFeature(input);
	shapeVariance = getShapeVariance(input, 10);
	avgAngleChange = getAverageAngleChange(input, 10);
	perimAreaRatio = getPerimeterToAreaRatio(input);
	boundBoxAspRatio = getBoundBoxAspectRatio(input);
	distFromCenter = getDistToCenter(input);
	avgColourOrigImg = getAverageColourIntensity(input, orgSrc, 0);
}



inline bool ImgObj::Features::doesFiExists(const std::string& name) {
	if (std::FILE *file = std::fopen(name.c_str(), "r")) {
		std::fclose(file);
		return true;
	}
	else {
		return false;
	}
}

Mat ImgObj::Features::getFeaturAsMat() {
	Mat output = Mat(1, 8, CV_32FC1);
	output.at<float>(0, 0) = area;
	output.at<float>(0, 1) = arclength;
	output.at<float>(0, 2) = shapeVariance;
	output.at<float>(0, 3) = avgAngleChange;
	output.at<float>(0, 4) = perimAreaRatio;
	output.at<float>(0, 5) = boundBoxAspRatio;
	output.at<float>(0, 6) = distFromCenter;
	output.at<float>(0, 7) = avgColourOrigImg;
	return output;
}

void ImgObj::Features::writeFeaturesToFile(const std::string& className, Path inputFiPath = Path()) {
	// see if the file exists and if not, create it and make headder
	//if (!doesFiExists("./features/" + className + ".csv")) {
	//	ofstream writeoutfile("./features/" + className + ".csv");
	//}
	// format the data
	ofstream writeoutfile("./features/" + className + ".csv", std::ios_base::app);
	writeoutfile << std::fixed;
	writeoutfile << area << ",";
	writeoutfile << arclength << ",";
	writeoutfile << shapeVariance << ",";
	writeoutfile << avgAngleChange << ",";
	writeoutfile << perimAreaRatio << ",";
	writeoutfile << boundBoxAspRatio << ",";
	writeoutfile << distFromCenter << ",";
	writeoutfile << avgColourOrigImg << endl;

	ofstream writeoutfilelable("./features/" + className + "_labels.csv", std::ios_base::app);
	if (inputFiPath.filename().substr(0, 2) == className) {
		writeoutfilelable << "1" << ",";
	}
	else {
		writeoutfilelable << "0" << ",";
	}
}

void ImgObj::Features::coutData() {
	//Cout vars for debugging
	cout << name << " category" << std::fixed << endl;
	cout << "Area of issue: " << area << endl;
	cout << "Arclengths of issue: " << arclength << endl;
	cout << "Shape variance of issue: " << shapeVariance << endl;
	cout << "Average angle change of issue: " << avgAngleChange << endl;
	cout << "Perimeter/area ratio of issue: " << perimAreaRatio << endl;
	cout << "Boundary box of issue aspect ratio: " << boundBoxAspRatio << endl;
	cout << "Distance indicator: " << distFromCenter << endl;
	cout << "Average colour: " << avgColourOrigImg << endl << endl;
}
