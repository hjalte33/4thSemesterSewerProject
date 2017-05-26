#include "stdafx.h"
#include "Algorithms.h"
#include "Path.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace cv;
using namespace std;


// Translate an image in x and y
void translateImg(Mat img, int offsetx, int offsety) {
	// translate an image with offset x and y 
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
}

void showImages(Mat image1, Mat image2, Mat image3, Size size, int waitkey) {

	resize(image1, image1, size);
	resize(image2, image2, size);
	resize(image3, image3, size);


	imshow("Image 1", image1);
	imshow("Image 2", image2);
	imshow("Image 3", image3);

	waitKey(waitkey);

}

long int CountPixelBrightness(cv::Mat inputImage) {
	long int brightness = 0;

	for (int x = 0; x < inputImage.cols; x++) {
		for (int y = 0; y < inputImage.rows; y++) {
			brightness += inputImage.at<uchar>(Point(x, y));
		}
	}

	return brightness;
}

long int CountWhitePixels(cv::Mat inputImage) {
	long int whitePixels = 0;

	for (int x = 0; x < inputImage.cols; x++) {
		for (int y = 0; y < inputImage.rows; y++) {
			if (inputImage.at<uchar>(Point(x, y)) > 100) {
				whitePixels++;
			}
		}
	}

	return whitePixels;
}

struct RGB {        //members are in "bgr" order!
	uchar blue;
	uchar green;
	uchar red;
};


//converts an rgb to grayscale using the the RGB weights specified. The weights are normalised internaly
Mat rgbToGray(cv::Mat src, float red, float green, float blue) {
	float norm = 1 / (red + green + blue);
	float _red = red * norm;
	float _green = green * norm;
	float _blue = blue * norm;
	Mat grayscale = Mat(src.rows, src.cols, 0);

	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			RGB& rgb = src.ptr<RGB>(row)[col]; //y = row, x = col
			uchar& gray = grayscale.ptr<uchar>(row)[col];
			//int brightness = (rgb.blue + rgb.green + rgb.red) / 3;
			grayscale.ptr<uchar>(row)[col] = rgb.blue * _blue + rgb.green * _green + rgb.red * _red;
		}
	}
	return grayscale;
}

//converts an rgb to grayscale using the BGR channel specified
Mat rgbToGray(Mat src, int channel) {
	Mat bgr[3];
	split(src, bgr);
	return bgr[channel];

}


int getThresh(Mat inputImage) {
	Scalar scalthresh = mean(inputImage);
	int thresh = round(scalthresh.val[0]);
	return thresh;
}


Mat keepBlobBiggestArea(Mat inputImage) {
	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	int biggestBlob = 0;
	long int area = 0;
	long int lastArea = 0;


	for (int i = 0; i < contours.size(); i++) {
		area = contourArea(contours[i]);

		if (area < lastArea) {
			drawContours(inputImage, contours, i, Scalar(0), FILLED);	
		}
		else {
			if (i != 0) { 
				drawContours(inputImage, contours, biggestBlob, Scalar(0), FILLED); };
			biggestBlob = i;
			lastArea = area;
		}

	}

	return inputImage;
}


/// Find contours twice to remove the middle cirkle of the image.
Mat keepBlobBiggestEnclosingCircle(Mat inputImage) {

	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	RNG rng(12345);

	/// Approximate contours to polygons + get bounding rects and circles	
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Point2f>center(contours.size());
	vector<float>radius(contours.size());
	float areaPrev = 0;
	int biggestCirkleIndex = 0;
	for (int i = 0; i < contours.size(); i++) {
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
		float circleArea = M_PI * radius[i] * radius[i];
		if (circleArea < areaPrev) {
			drawContours(inputImage, contours, i, Scalar(0), -1);
		}
		else {
			areaPrev = circleArea;
			if (i != 0) {
				drawContours(inputImage, contours, biggestCirkleIndex, Scalar(0), -1);
			}
			biggestCirkleIndex = i;
		}
	}

	return (inputImage);
}


// ------------------------------------------------------------------------
// ------------------- Segmentation functions -----------------------------
// ------------------------------------------------------------------------


Mat ROSegmentation(Mat src, Mat refImage) {

	Mat refImgCopy = rgbToGray(refImage.clone(), 0, 0, 1);
	src = rgbToGray(src, 0, 0, 1); // get blue channel

	//Blurring the background image for substracting
	int KernelSize = 30;
	blur(refImgCopy, refImgCopy, Size(KernelSize, KernelSize));

	//Substracting the two images and save it to diff
	Mat diff = src - refImgCopy;

	//Making the diff image binary
	threshold(diff, diff, 135, 255, CV_THRESH_BINARY);
	namedWindow("ROE", WINDOW_KEEPRATIO);
	imshow("ROE", diff);
	waitKey(1);
	return diff;
}

Mat FSSegmentation(Mat src, Mat refImage) {

	Mat redImage = rgbToGray(src,1,0,0); // 
	equalizeHist(redImage, redImage);
	threshold(redImage, redImage, 60, 255, THRESH_BINARY_INV);
	
	Mat elementOpen = getStructuringElement(MORPH_RECT, Size(8, 8));
	morphologyEx(redImage, redImage, MORPH_OPEN, elementOpen);

	Mat contours1 = keepBlobBiggestEnclosingCircle(redImage);
	namedWindow("FS", WINDOW_KEEPRATIO);
	imshow("FS", contours1);
	waitKey(1);
	return contours1;
	//Mat convexImage = findconvex(contours1);
}

Mat RBSegmentation(Mat src, Mat refImage) {
	Mat image;
	Mat ref;

	image = rgbToGray(src,2);
	ref = rgbToGray(refImage,2);

	Mat diff;
	subtract(ref,image, diff) ;
	
	int KernelSize = 30;
	blur(diff, image, Size(KernelSize, KernelSize));

	int thresval = getThresh(image);

	threshold(image, image, thresval * 11, 255, THRESH_BINARY);

	image = keepBlobBiggestArea(image);
	
	namedWindow("RB", WINDOW_KEEPRATIO);
	imshow("RB", image);
	waitKey(1);
	
	return image;
}



// ------------------------------------------------------------------------
// -------------------- Classification functions --------------------------
// ------------------------------------------------------------------------


long int getAreaFeature(Mat inputImage) {
	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	long int area = 0;
	long int areaKeep = 0;
	long int lastarea = 0;

	for (int i = 0; i < contours.size(); i++) {
		area = contourArea(contours[i]);
		if (area > lastarea) {
			areaKeep = contourArea(contours[i]);
			lastarea = area;
		}
	}

	return areaKeep;
}

long int getArclengthFeature(Mat inputImage) {

	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	long int arclength = 0;
	long int arclengthKeep = 0;
	long int lastArclength = 0;

	for (int i = 0; i < contours.size(); i++) {
		arclength = contourArea(contours[i]);
		if (arclength > lastArclength) {
			arclengthKeep = arcLength(contours[i], true);

			lastArclength = arclength;
		}
	}


	return arclengthKeep;
}

float getShapeVariance(Mat inputImage, int pd) {

	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size() < 1) return 0;

	int b = 0;
	if (contours.size() > 1)
	{
		for (int i = 1; i < contours.size(); i++) {
			if (contours[i].size() > contours[i - 1].size()) {
				b = i;
			}
		}
	}


	double Variance = 0;
	double xdif = 0;
	double ydif = 0;
	double tangentAngle1 = 0;
	double tangentAngle2 = 0;
	double TangentDifference = 0;
	double SumofTangentDifference = 0;
	double AverageTangentDifference = 0;
	double squareddifference = 0;
	long double sum = 0;
	long int n = (contours[b].size() - pd - 1);



	if (n < 1)
	{
		return 0;
	}

	for (int x = 0; x < n; x++)
	{

		xdif = contours[b][x + pd].x - contours[b][x].x;
		ydif = contours[b][x + pd].y - contours[b][x].y;

		tangentAngle1 = atan2(ydif, xdif);

		//cout << tangentAngle1 << "tangentangle1" << endl;
		xdif = contours[b][x + pd + 1].x - contours[b][x + 1].x;
		ydif = contours[b][x + pd + 1].y - contours[b][x + 1].y;
		tangentAngle2 = atan2(ydif, xdif);

		TangentDifference = abs(tangentAngle1 - tangentAngle2);
		if (TangentDifference < M_PI)
			SumofTangentDifference = SumofTangentDifference + TangentDifference;
		else
			SumofTangentDifference = SumofTangentDifference + (2 * M_PI - TangentDifference);
	}


	AverageTangentDifference = SumofTangentDifference / n;


	for (int x = 0; x < n; x++)
	{

		xdif = contours[b][x + pd].x - contours[b][x].x;
		ydif = contours[b][x + pd].y - contours[b][x].y;


		tangentAngle1 = atan2(ydif, xdif);

		//cout << tangentAngle1 << "tangentangle1" << endl;
		xdif = contours[b][x + pd + 1].x - contours[b][x + 1].x;
		ydif = contours[b][x + pd + 1].y - contours[b][x + 1].y;

		tangentAngle2 = atan2(ydif, xdif);
		//cout << tangentAngle2 << "tangentangle2" << endl;
		TangentDifference = abs(tangentAngle1 - tangentAngle2);
		if (TangentDifference > M_PI)
		{
			TangentDifference = (2 * M_PI - TangentDifference);
		}
		//cout << tangentAngle1 << "Angle1" << endl;
		//	cout << tangentAngle2 << "Angle2" << endl;
		//cout << TangentDifference << "Tangent Difference" << endl;

		squareddifference = ((TangentDifference - AverageTangentDifference) * (TangentDifference - AverageTangentDifference));
		sum = sum + squareddifference;
	}

	// n used instead of n-1 as variance is of the whole population and not a sample.
	Variance = sum / n;

	//cout << Variance << " Variance" << endl;
	return Variance;
}

float getAverageAngleChange(Mat inputImage, int pd) {
	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

	if (contours.size() < 1) return 0;

	int b = 0;
	if (contours.size() > 1)
	{
		for (int i = 1; i < contours.size(); i++) {
			if (contours[i].size() > contours[i - 1].size()) {
				b = i;
			}
		}
	}

	double xdif = 0;
	double ydif = 0;
	double tangentAngle1 = 0;
	double tangentAngle2 = 0;
	double TangentDifference = 0;
	double SumofTangentDifference = 0;
	double AverageTangentDifference = 0;
	long int n = (contours[b].size() - pd - 1);

	if (n < 1)
	{
		return 0;
	}

	for (int x = 0; x < n; x++)
	{

		xdif = contours[b][x + pd].x - contours[b][x].x;
		ydif = contours[b][x + pd].y - contours[b][x].y;

		tangentAngle1 = atan2(ydif, xdif);

		//cout << tangentAngle1 << "tangentangle1" << endl;
		xdif = contours[b][x + pd + 1].x - contours[b][x + 1].x;
		ydif = contours[b][x + pd + 1].y - contours[b][x + 1].y;
		tangentAngle2 = atan2(ydif, xdif);

		TangentDifference = abs(tangentAngle1 - tangentAngle2);
		if (TangentDifference < M_PI)
			SumofTangentDifference = SumofTangentDifference + TangentDifference;
		else
			SumofTangentDifference = SumofTangentDifference + ((2 * M_PI) - TangentDifference);
	}


	AverageTangentDifference = SumofTangentDifference / n;

	return AverageTangentDifference;
}

float getPerimeterToAreaRatio(Mat inputImage) {
	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	
	if (contours.size() < 1) return 0;
	int b = 0;
	if (contours.size() > 1)
	{	
		for (int i = 1; i < contours.size(); i++) {
			if (contours[i].size() > contours[i - 1].size()) {
				b = i;
			}
		}
	}

	double Ratio = 0;
	if ((arcLength(contours[b], false) == 0) || (contourArea(contours[b], false) == 0))
	{
		return 0;
	}

	Ratio = arcLength(contours[b], false) / contourArea(contours[b], false);
	return Ratio;
}

float getBoundBoxAspectRatio(Mat inputImage) {
	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	if (contours.size() < 1) return 0;
	int b = 0;
	if (contours.size() > 1){
		for (int i = 1; i < contours.size(); i++) {
			if (contours[i].size() > contours[i - 1].size()) {
				b = i;
			}
		}
	}
	Rect rect = boundingRect(contours[b]);

	return (1.0 * rect.height) / rect.width;
}

float getDistToCenter(Mat inputImage) {

	vector<vector<Point>> contours;
	findContours(inputImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	long int area = 0;
	long int lastarea = 0;
	int distC = 0;
	int id = 0;
	
	if (contours.size() > 0) {
	for (int i = 0; i < contours.size(); i++) {
		area = contourArea(contours[i]);
		if (area > lastarea) {
			id = i;
			lastarea = area;
		}
	}

	
		Point issueC, imgC;

		issueC.x = moments(contours[id]).m10 / moments(contours[id]).m00;
		issueC.y = moments(contours[id]).m01 / moments(contours[id]).m00;
		imgC.x = inputImage.cols / 2;
		imgC.y = inputImage.rows / 2;
		distC = norm(issueC - imgC);
	}

	return distC;
}

float getAverageColourIntensity(Mat inputImage, Mat originalImage, int Channel) {

	Scalar avgcolourvalue;

	avgcolourvalue = mean(originalImage, inputImage);

	return avgcolourvalue.val[Channel];
}

