#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include "detector.h"


cv::Ptr<cv::BackgroundSubtractor> backgroundsubtractor(std::string subtype, bool shadow) {
	cv::Ptr<cv::BackgroundSubtractor> pBackSub;
	if (subtype == "MOG2")
		//pBackSub = cv::createBackgroundSubtractorMOG2(128, cv::THRESH_BINARY, 1);
		pBackSub = cv::createBackgroundSubtractorMOG2(200, 30);
	else if (subtype == "KNN")
		pBackSub = cv::createBackgroundSubtractorKNN(300, 600.0, shadow);

	return pBackSub;
}


void getContours(cv::Mat image, cv::Mat new_image, bool show_hull) {

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;

	cv::Mat kernel5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
	cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 5));
	cv::dilate(image, image, kernel3);
	cv::dilate(image, image, kernel3);
	cv::erode(image, image, kernel5);

	cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	// find area of contours
	std::vector<std::vector<cv::Point>> conPoly(contours.size());
	std::vector<cv::Rect> boundRect(contours.size());
	std::vector<std::vector<cv::Point>> hull(contours.size());

	cv::Mat imgHull(image.size(), CV_8UC3, cv::Scalar(0.0, 0.0, 0.0));

	for (int i = 0; i < contours.size(); i++) {
		int area = cv::contourArea(contours[i]);
		//std::cout << area << std::endl;

		//cv::convexHull(contours[i], hull[i]);
		//int area = cv::contourArea(hull[i]);

		if (area > 400) {
			float peri = cv::arcLength(contours[i], true);
			cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			//boundRect[i] = cv::boundingRect(conPoly[i]);
			//cv::rectangle(new_image, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 2);
			//cv::drawContours(new_image, conPoly, i, cv::Scalar(0, 0, 255), -1);
			cv::convexHull(conPoly[i], hull[i]);
			boundRect[i] = cv::boundingRect(hull[i]);
			cv::drawContours(imgHull, hull, i, cv::Scalar(255.0, 255.0, 255.0), -1);
			cv::rectangle(new_image, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 2);
		}
	}

	if (show_hull)
		cv::imshow("ConvexHull Image", imgHull);
}