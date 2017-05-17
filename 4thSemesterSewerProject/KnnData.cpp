#include "stdafx.h"
#include "KnnData.h"
#include "opencv2\ml.hpp"

using namespace std;
using namespace cv;
using namespace ml;



Mat KnnData::getMinValues(Mat inputMatrix) {
	minValues = Mat(1, inputMatrix.cols, CV_32FC1);
	for (int x = 0; x < inputMatrix.cols; x++) {
		minValues.at<float>(0, x) = 10000000000;
		for (int y = 0; y < inputMatrix.rows; y++) {
			if (inputMatrix.at<float>(y, x) < minValues.at<float>(0, x)) {
				minValues.at<float>(0, x) = inputMatrix.at<float>(y, x);
			}
		}
	}
	return minValues;
}

Mat KnnData::getMaxValues(Mat inputMatrix) {
	maxValues = Mat(1, inputMatrix.cols, CV_32FC1);
	for (int x = 0; x < inputMatrix.cols; x++) {
		maxValues.at<float>(0, x) = -1;
		for (int y = 0; y < inputMatrix.rows; y++) {
			if (inputMatrix.at<float>(y, x) > maxValues.at<float>(0, x)) {
				maxValues.at<float>(0, x) = inputMatrix.at<float>(y, x);
			}
		}
	}
	return maxValues;
}

Mat KnnData::NormaliseDataCol(Mat_<float> inputMatrix, Mat minValues, Mat maxValues) {
	Mat_<float> outputMatrix(inputMatrix.rows, inputMatrix.cols, CV_32FC1);
	for (int x = 0; x < inputMatrix.cols; x++) {
		for (int y = 0; y < inputMatrix.rows; y++) {
			outputMatrix.at<float>(y, x) = (inputMatrix.at<float>(y, x) - minValues.at<float>(0, x)) / (maxValues.at<float>(0, x) - minValues.at<float>(0, x));
		}
	}
	return outputMatrix;
}

Mat KnnData::NormaliseDataCol(Mat_<float> inputMatrix) {
	Mat_<float> outputMatrix(inputMatrix.rows, inputMatrix.cols, CV_32FC1);
	maxValues = getMaxValues(inputMatrix);
	minValues = getMinValues(inputMatrix);

	for (int x = 0; x < inputMatrix.cols; x++) {
		for (int y = 0; y < inputMatrix.rows; y++) {
			outputMatrix.at<float>(y, x) = (inputMatrix.at<float>(y, x) - minValues.at<float>(0, x)) / (maxValues.at<float>(0, x) - minValues.at<float>(0, x));
		}
	}
	return outputMatrix;
}


//constructor. 
KnnData::KnnData(std::string _trainingDataPath, std::string _labelsDataPath) {
	

	// read the training data from this file
	ifstream trainingDataPath(_trainingDataPath);


	Mat_<float> trainingData(0, 8, CV_32FC1);
	for (string line; getline(trainingDataPath, line);) {
		stringstream ss(line);

		Mat_<float> row(1, 8, CV_32FC1);
		string substr;
		for (int i = 0; getline(ss, substr, ','); i++) {
			row.at<float>(0, i) = stof(substr);
		}
		trainingData.push_back(row);
	}

	//normalize data
	trainingDataNorm = NormaliseDataCol(trainingData);

	ifstream trainingDataLablesPath(_labelsDataPath);
	string line;
	getline(trainingDataLablesPath, line);
	stringstream ss(line);
	vector<float> trainLabels;
	while (ss.good()) {
		string substr;
		getline(ss, substr, ',');
		trainLabels.push_back(stof(substr));
	}
	
	knn->train(trainingDataNorm, ROW_SAMPLE, trainLabels);
}

float KnnData::findKnn(Mat_<float> featureToTest) {
	Mat_<float> response;
	Mat_<float> dists;

	Mat_<float> testFeatureNorm = NormaliseDataCol(featureToTest, minValues, maxValues);
	knn->findNearest(testFeatureNorm, K, noArray(), response, dists);

	float score = 0;
	for (int i = 0; i < K; i++){
		if (response.at<float>(0, i) == 1) {
			score += 1 / (1 + dists.at<float>(0, i));
		}
		else {
			score -= 1 / (1 + dists.at<float>(0, i));
		}
	}

	
	return score;
}

