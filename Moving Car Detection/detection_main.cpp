#include <iostream> 
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include "detector.h"


void main() {

	std::string path = "Test Data/Highway.mp4";
	cv::VideoCapture cap(path); // 0 as webcam id
	cv::Mat img;

	cv::Ptr<cv::BackgroundSubtractor> subtractor;
	subtractor = backgroundsubtractor("MOG2", true);

	cv::Rect roi(0, 100, 426, 130);
	cv::Mat mask, imgCrop;
	while (true) {
		cap.read(img);
		imgCrop = img(roi);
		subtractor->apply(imgCrop, mask);
		mask.setTo(0, mask < 254);
		getContours(mask, imgCrop, true);

		//std::cout << img.size() << std::endl;
		cv::imshow("video", img);
		//cv::imshow("cropped", imgCrop);
		cv::imshow("mask", mask);
		cv::waitKey(10);
	}
}