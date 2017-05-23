#include "stdafx.h"
#include "ImgObj.h"
#include "Algorithms.h"
#include "Path.h"

// reads the images and checks if they are read correctly
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

// constructor
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
	Features FSFeatures = Features("FS", FSSegmentation(src, ref), src);
	Features ROFeatures = Features("RO", ROSegmentation(src, ref), src);
	Features RBFeatures = Features("RB", RBSegmentation(src, ref), src);

	// accquire training data
	FSFeatures.writeFeaturesToFile("FS", srcPath.completepath);
	ROFeatures.writeFeaturesToFile("RO", srcPath.completepath);
	RBFeatures.writeFeaturesToFile("RB", srcPath.completepath);

	//cout for debugging
	cout << "Image name " << srcPath.filename() << endl;
	ROFeatures.coutData();
	RBFeatures.coutData();
	FSFeatures.coutData();
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
	// the name that is passed the the fuction must be exatly the same two letters 
	// as the first two letters of the file name. This is used when the labels 
	// are written. 
	Features FSFeatures = Features("FS", FSSegmentation(src, ref), src);
	Features ROFeatures = Features("RO", ROSegmentation(src, ref), src);
	Features RBFeatures = Features("RB", RBSegmentation(src, ref), src);

	featuresMat.push_back(FSFeatures.getFeaturAsMat());
	featuresMat.push_back(ROFeatures.getFeaturAsMat());
	featuresMat.push_back(RBFeatures.getFeaturAsMat());
}

cv::Mat ImgObj::getFeaturesMat()
{
	return featuresMat;
}



//----------------------------------------------------------------------------
//---------------------- Feature struct fuctions/constructors ----------------
//----------------------------------------------------------------------------

// constructor calculates all the fetures and populates the struct 
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
	// format the data
	ofstream writeoutfile(inputFiPath.dir() + "../features/" + className + ".csv", std::ios_base::app);
	writeoutfile << std::fixed;
	writeoutfile << area << ",";
	writeoutfile << arclength << ",";
	writeoutfile << shapeVariance << ",";
	writeoutfile << avgAngleChange << ",";
	writeoutfile << perimAreaRatio << ",";
	writeoutfile << boundBoxAspRatio << ",";
	writeoutfile << distFromCenter << ",";
	writeoutfile << avgColourOrigImg << endl;

	string labelFiName = inputFiPath.dir() + "../features/" + className + "_labels.csv";
	//if the file does exists, append a comma and a no, else write only the number.
	if (doesFileExist(labelFiName)) {
		ofstream writeoutfilelable(labelFiName, std::ios_base::app);
		//check if the class name maches the file name and write the label file
		if (inputFiPath.filename().substr(0, 2) == className) {
			writeoutfilelable << "," << "1" ;
		}
		else {
			writeoutfilelable << "," << "0" ;
		}
	}
	else {	
		ofstream writeoutfilelable(labelFiName, std::ios_base::app);
		//check if the class name maches the file name and write the label file
		if (inputFiPath.filename().substr(0, 2) == className) {
			writeoutfilelable << "1" ;
		}
		else {
			writeoutfilelable << "0" ;
		}
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


inline bool ImgObj::Features::doesFileExist(const std::string& name) {
	if (FILE *file = fopen(name.c_str(), "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}