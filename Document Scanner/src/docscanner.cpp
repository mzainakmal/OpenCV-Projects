#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "docscanner.h"



cv::Mat preprocess(cv::Mat img) {
	cv::Mat imgGray, imgBlur, imgCanny, imgDil;
	cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
	cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
	cv::Canny(imgBlur, imgCanny, 25, 75);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
	cv::dilate(imgCanny, imgDil, kernel);
	return imgDil;
}


std::vector<cv::Point> getContours(cv::Mat image, cv::Mat new_image) {

	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	std::vector<cv::Point> biggest;

	cv::findContours(image, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// find area of contours
	std::vector<std::vector<cv::Point>> conPoly(contours.size());

	int maxArea = 0;
	for (int i = 0; i < contours.size(); i++) {
		int area = cv::contourArea(contours[i]);
		//std::cout << area << std::endl;

		if (area > 1000) {
			float peri = cv::arcLength(contours[i], true);
			cv::approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {
				maxArea = area;
				biggest = conPoly[i];
				//biggest = { conPoly[i][0], conPoly[i][1], conPoly[i][2], conPoly[i][3] };
			}
			//cv::drawContours(new_image, conPoly, i, cv::Scalar(0, 0, 255), 2);
		}
	}
	return biggest;
}


void drawPoints(std::vector<cv::Point> points, cv::Scalar colour, cv::Mat image) {
	for (int i = 0; i < points.size(); i++) {
		cv::circle(image, points[i], 7, colour, cv::FILLED);
		cv::putText(image, std::to_string(i), points[i], cv::FONT_HERSHEY_COMPLEX, 1, colour, 2);
	}
}


std::vector<cv::Point> reorderPoints(std::vector<cv::Point> points) {
	std::vector<cv::Point> newPoints;
	std::vector<int> sumPoints, subPoints;

	for (int i = 0; i < points.size(); i++) {
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[std::min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);
	newPoints.push_back(points[std::max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(points[std::min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
	newPoints.push_back(points[std::max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);

	return newPoints;
}


cv::Mat imgWarper(cv::Mat img, std::vector<cv::Point> points, float w, float h) {
	cv::Mat imwarp;
	cv::Point2f src[4] = { points[0], points[1], points[2], points[3] };
	cv::Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w,h} };

	cv::Mat matrix = cv::getPerspectiveTransform(src, dst);
	cv::warpPerspective(img, imwarp, matrix, cv::Point(w, h));
	return imwarp;
}

