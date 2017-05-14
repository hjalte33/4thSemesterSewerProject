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

void ImgObj::calculateScores() {
	// make sure that the images are loaded
	if (!isImagesLoaded) {
		readImages();
	}
	namedWindow("working on", WINDOW_KEEPRATIO);
	imshow("working on", src);
	waitKey(1);

	Features FSFeaturs = Features("FS", FSSegmentation(src, ref), src);
	Features ROEFeaturs = Features("ROE", ROESegmentation(src, ref), src);
	Features RBFeaturs = Features("RB", RBSegmentation(src, ref), src);

	FSFeaturs.writeFeaturesToFile("FS");
	ROEFeaturs.writeFeaturesToFile("ROE");
	RBFeaturs.writeFeaturesToFile("RB");

	cout << "Image name " << srcPath.filename() << endl;
	ROEFeaturs.coutData();
	RBFeaturs.coutData();
	FSFeaturs.coutData();

	//here it should mach with training data. 

}


ImgObj::Features::Features(std::string _name, cv::Mat input, cv::Mat orgSrc) {
	name = _name;
	area = getAreaFeature(input);
	arclength = getArclengthFeature(input);
	shapeVariance = getShapeVariance(input, 10);
	avgAngleChange = AverageAngleChange(input, 10);
	PerimAreaRatio = PerimeterToAreaRatio(input);
	BoundBoxAspRatio = boundBoxAspectRatio(input);
	distFromCenter = getDistToCenter(input);
	AvgColourOrigImg = AverageColourIntensity(input, orgSrc, 0);
}



inline bool doesFiExists(const std::string& name) {
	if (std::FILE *file = std::fopen(name.c_str(), "r")) {
		std::fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void ImgObj::Features::writeFeaturesToFile(const std::string& fiName) {
	// see if the file exists
	if (!doesFiExists("./features/" + fiName + ".csv")) {
		ofstream writeoutfile("./features/" + fiName + ".csv");
		writeoutfile << "Area of issue," ;
		writeoutfile << "Arclengths of issue," ;
		writeoutfile << "Shape variance of issue," ;
		writeoutfile << "Average angle change of issue," ;
		writeoutfile << "Perimeter/area ratio of issue," ;
		writeoutfile << "Boundary box aspect ratio," ;
		writeoutfile << "Distance indicator,";
		writeoutfile << "Average colour," << endl;
	}
	// format the data
	ofstream writeoutfile("./features/" + fiName + ".csv", std::ios_base::app);
	writeoutfile << area << ",";
	writeoutfile << arclength << ",";
	writeoutfile << shapeVariance << ",";
	writeoutfile << avgAngleChange << ",";
	writeoutfile << PerimAreaRatio << ",";
	writeoutfile << boundBoxAspectRatio << ",";
	writeoutfile << distFromCenter << ",";
	writeoutfile << AvgColourOrigImg << endl;
}

void ImgObj::Features::coutData() {
	//Cout vars
	cout << name << " category" << endl;
	cout << "Area of issue: " << area << endl;
	cout << "Arclengths of issue: " << arclength << endl;
	cout << "Shape variance of issue: " << shapeVariance << endl;
	cout << "Average angle change of issue: " << avgAngleChange << endl;
	cout << "Perimeter/area ratio of issue: " << PerimAreaRatio << endl;
	cout << "Boundary box of issue aspect ratio: " << BoundBoxAspRatio << endl;
	cout << "Distance indicator: " << distFromCenter << endl;
	cout << "Average colour: " << AvgColourOrigImg << endl << endl;
}
