#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "docscanner.h";


void main() {
	cv::Mat imgWarp, imgCrop, imgSharp;
	std::vector<cv::Point> initPoints, orderedPoints;

	std::string path = "Test Data/Document.jpg";
	cv::Mat img = cv::imread(path);
	//cv::resize(img, img, cv::Size(), 0.75, 0.75);

	// Preprocessing
	cv::Mat imgProcessed = preprocess(img);
	//cv::imwrite("Test Data/Processed Doc.jpg", imgProcessed);

	// Get Contours
	initPoints = getContours(imgProcessed, img);
	//drawPoints(initPoints, cv::Scalar(0, 0, 150), img);
	orderedPoints = reorderPoints(initPoints);
	//drawPoints(orderedPoints, cv::Scalar(200, 0, 0), img);

	// Warp
	float w = 420, h = 596;  // using w and h of an A4 paper
	imgWarp = imgWarper(img, orderedPoints, w, h);
	//cv::imwrite("Test Data/Warped Doc.jpg", imgWarp);

	// Crop
	int cropVal = 5;
	cv::Rect roi(cropVal, cropVal, w - cropVal, h - cropVal);
	imgCrop = imgWarp(roi);

	// Sharpening
	//cv::Mat kernel = (cv::Mat_<double>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	//cv::filter2D(imgCrop, imgSharp, -1, kernel);
	cv::GaussianBlur(imgCrop, imgSharp, cv::Size(0, 0), 3);
	cv::addWeighted(imgCrop, 1.5, imgSharp, -0.5, 0, imgSharp);

	// Saving image
	//cv::imwrite("Test Data/Scanned Document.jpg", imgSharp);
	//std::cout << "Scanned Document Saved!!" << std::endl;

	cv::imshow("original image", img);
	cv::imshow("processed image", imgProcessed);
	cv::imshow("warped image", imgWarp);
	cv::imshow("cropped image", imgCrop);
	cv::imshow("sharpened image", imgSharp);
	cv::waitKey(0);

}