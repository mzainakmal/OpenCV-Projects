#pragma once

cv::Mat preprocess(cv::Mat img);
std::vector<cv::Point> getContours(cv::Mat image, cv::Mat new_image);
void drawPoints(std::vector<cv::Point> points, cv::Scalar colour, cv::Mat image);
std::vector<cv::Point> reorderPoints(std::vector<cv::Point> points);
cv::Mat imgWarper(cv::Mat img, std::vector<cv::Point> points, float w, float h);