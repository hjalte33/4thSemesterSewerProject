#include "Algorithms.h"
#include "stdafx.h"

using namespace cv;
using namespace std;



// Translate an image in x and y
void translateImg(Mat img, int offsetx, int offsety) {
	// translate an image with offset x and y 
	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, offsetx, 0, 1, offsety);
	warpAffine(img, img, trans_mat, img.size());
}

/* Tries to find the smallest cirkle bigger than minRadius
and then center the image at that */
void centering(Mat src, int minRadius, int step = 2) {
	//first create a blured image to reduce some noice
	Mat blured;
	blur(src, blured, Size(9, 9));
	//vector to store the circles found
	vector<Vec3f> circles;

	//inrease the circle diametre untill a circle is found. 
	for (int i = minRadius; circles.size() < 1 && i < blured.rows / 2; i += step) {
		HoughCircles(blured, circles, CV_HOUGH_GRADIENT, 1, blured.rows / 6, 10, 100, minRadius, i);
	}

	//translate the image so it's centered
	int shiftx = circles[0][0] - src.cols / 2;
	int shifty = circles[0][1] - src.cols / 2;
	translateImg(src, shiftx, shifty);
	//cout << shiftx << "  " << shifty << endl;   //debugging
}

Mat equalizeIntensity(const Mat& inputImage)
{
	if (inputImage.channels() >= 3)
	{
		Mat ycrcb;

		cvtColor(inputImage, ycrcb, CV_BGR2YCrCb);

		vector<Mat> channels;
		split(ycrcb, channels);

		equalizeHist(channels[0], channels[0]);

		Mat result;
		merge(channels, ycrcb);

		cvtColor(ycrcb, result, CV_YCrCb2BGR);

		return result;
	}
	return Mat();
}


//Find the root and returns an image with the result.
Mat findRoot(cv::Mat background, cv::Mat src) {
	//substract the background

	Mat result;
	Mat blured;
	int kernelsize = 51;
	//GaussianBlur(background,blured,Size(kernelsize,kernelsize),1);
	//absdiff(src, blured, result);
	Mat resultfiltered;
	bilateralFilter(src, resultfiltered, 12, 50,50);
	cvtColor(resultfiltered,resultfiltered, CV_BGR2GRAY);
	Canny(resultfiltered, resultfiltered, 180, 75, 5);
	return resultfiltered;
}


struct RGB {        //members are in "bgr" order!
	uchar blue;
	uchar green;
	uchar red;
};

cv::Mat rgbToGray(cv::Mat src) {
	Mat grayscale = Mat(src.rows, src.cols, CV_8U);
	for (int row = 0; row < src.rows; row++) {
		for (int col = 0; col < src.cols; col++) {
			RGB& rgb = src.ptr<RGB>(row)[col]; //y = row, x = col
			uchar& gray = grayscale.ptr<uchar>(row)[col];
			//int brightness = (rgb.blue + rgb.green + rgb.red) / 3;
			grayscale.ptr<uchar>(row)[col] = rgb.blue * 0.2 + rgb.green * 0.2 + rgb.red * 0.6;
		}
	}
	return grayscale;
}

Mat getRedChannel(Mat inputImage) {
	Mat bgr[3];
	split(inputImage, bgr);
	return bgr[2];
}

Mat findRoot3(cv::Mat& background, cv::Mat& src) {
	Mat srcBW = getRedChannel(src);
	Mat srcBWBlur = getRedChannel(src);
	Mat backBWBlur = getRedChannel(background);
	int kernalsize = 81;
	//equalizeHist(srcBWBlur, srcBWBlur);
	//equalizeHist(backBWBlur, backBWBlur);
	GaussianBlur(backBWBlur, backBWBlur, Size(kernalsize, kernalsize), 10);
	kernalsize = 41;
	GaussianBlur(srcBW, srcBWBlur, Size(kernalsize, kernalsize), 10);
	Mat mask;
	absdiff(srcBWBlur, backBWBlur, mask);
	
	int elementsize = 20;
	Mat element = getStructuringElement(0, Size(elementsize, elementsize));
	morphologyEx(mask, mask, MORPH_OPEN, element);	
	bitwise_not(mask, mask);
	Mat result;
	result = srcBW - mask;
	
	return result;

}

struct HSV {        //members are in "bgr" order!
	uchar hue;
	uchar satuation;
	uchar value;
};

cv::Mat findRoot4(cv::Mat background, cv::Mat src) {
	Mat src_gray;
	Mat contours;
	Mat dx, dy;
	cv::Canny(src, contours, 10, 350);
	cv::Sobel(src, dx, CV_64F, 1, 0, 3, 1, 0, cv::BORDER_REPLICATE);
	cv::Sobel(src, dy, CV_64F, 0, 1, 3, 1, 0, cv::BORDER_REPLICATE);

	// create and convert RGB image to HSV
	cv::Mat angle(src.size(), CV_64F);
	cvtColor(angle, angle, CV_BGR2HSV);

	//for (int i = 0; i < contours.rows; i++) {
	//	for (int j = 0; j < contours.cols; i++) {
	//		HSV& ang = angle.ptr<HSV>(i)[j];
	//		uchar& cont = contours.ptr<uchar>(i)[j];
	//		if (contours.ptr<uchar>(i)[j] > 0) {
	//			//angle.ptr<HSV>(i)[j].
	//		}
	//	}
	//}

	//	foreach(i, j) such that contours[i, j] > 0
	//{
	//	angle[i, j] = atan2(dy[i, j], dx[i, j])
	//}
	return src;
}

void findingContours(cv::Mat src) {
	vector<vector<Point>> contours;
	findContours(src, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	 
	double area = 0;
	double lastArea = 0;

	Point lastPt1, lastPt2;
	lastPt1.x = 0;
	lastPt1.y = 0;
	lastPt2.x = 1;
	lastPt2.y = 1;

	for (int i = 0; i<contours.size(); i++) {
		Rect rect = boundingRect(contours[i]);
		Point pt1, pt2;
		pt1.x = rect.x;
		pt1.y = rect.y;
		pt2.x = rect.x + rect.width;
		pt2.y = rect.y + rect.height;

		area = contourArea(contours[i]);

		if (area > lastArea) {
			//rectangle(newImgOut, pt1, pt2, Scalar(155), 1);
			cout << "Area of crack " << i << ": " << area << endl;
			cout << "point: " << "(" << pt1.x << "," << pt1.y << ")" << endl;
			cout << "Last point: " << "(" << lastPt1.x << "," << lastPt1.y << ")" << endl << endl;

			for (int x = lastPt1.x; x < lastPt2.x + 1; x++) {
				for (int y = lastPt1.y; y < lastPt2.y + 1; y++) {
					src.at<uchar>(y, x) = 0;
				}
			}

			lastArea = area;
			lastPt1.x = rect.x;
			lastPt1.y = rect.y;

			////These two lines make this function not work
			//lastPt2.x = pt2.x;
			//lastPt2.y = pt2.y;

		}


		else {
			//rectangle(newImgOut, pt1, pt2, Scalar(0), 1);
			for (int x = pt1.x; x < pt2.x; x++) {
				for (int y = pt1.y; y < pt2.y; y++) {
					src.at<uchar>(y, x) = 0;
				}
			}
		}
	}

	for (int i = 0; i < contours.size(); i++) {
		drawContours(src, contours, i, Scalar(155), 2);
	}

}


cv::Mat findRoot2(cv::Mat background, cv::Mat src){

	int kernelSize = 30;
	Mat blurred;
	//blur the background image
	blur(background, blurred, Size(kernelSize, kernelSize));

	//substract the background
	src = blurred - src;

	int elementSize = 14;
	//Morphology to reduce noise in the picture
	Mat element = getStructuringElement(0, Size(elementSize, elementSize));
	morphologyEx(src, src, MORPH_OPEN, element);
	morphologyEx(src, src, MORPH_CLOSE, element);

	cvtColor(src, src, CV_RGB2GRAY);
	threshold(src, src, 30, 255, THRESH_TRIANGLE);

	findingContours(src);
	return src;
}

Mat AlgorithmRoots(Mat backgroundImage, Mat inputImage) {

	//Making the inputImage greyscale
	Mat bgrInput[3];
	split(inputImage, bgrInput);
	inputImage = bgrInput[2];

	//Making the inputImage greyscale
	Mat bgrBackground[3];
	split(backgroundImage, bgrBackground);
	backgroundImage = bgrBackground[2];

	//Blurring the background image for substracting
	int KernelSize = 30;
	blur(backgroundImage, backgroundImage, Size(KernelSize, KernelSize));

	//Substracting the two images and save it to diff
	Mat diff = inputImage - backgroundImage;

	//Making the diff image binary
	int threshold = 45;
	for (int x = 0; x < diff.cols; x++) {
		for (int y = 0; y < diff.rows; y++) {
			if (diff.at<uchar>(Point(x, y)) < threshold) {
				diff.at<uchar>(Point(x, y)) = 0;
			}
			else {
				diff.at<uchar>(Point(x, y)) = 255;
			}
		}
	}

	return diff;
}
