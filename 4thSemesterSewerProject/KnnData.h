#ifndef KNNDATA_H
#define KNNDATA_H

#include <iostream>
#include "opencv2\core.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\ml.hpp"

class KnnData {
private:
	cv::Mat getMinValues(cv::Mat inputMatrix);
	cv::Mat getMaxValues(cv::Mat inputMatrix);
	cv::Mat NormaliseDataCol(cv::Mat_<float> inputMatrix, cv::Mat minValues, cv::Mat maxValues);
	cv::Mat NormaliseDataCol(cv::Mat_<float> inputMatrix);
	cv::Mat_<float> maxValues;
	cv::Mat_<float> minValues;
	cv::Mat_<float> trainingDataNorm;

	cv::Ptr<cv::ml::KNearest> knn = cv::ml::KNearest::create();

	const int K = 5;

public:
	KnnData(std::string _trainingDataPath, std::string _lablesDataPath);

	float findKnn(cv::Mat_<float> featureToTest);

};


#endif // !KNNDATA_H