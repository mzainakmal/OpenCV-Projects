#pragma once

cv::Ptr<cv::BackgroundSubtractor> backgroundsubtractor(std::string subtype, bool shadow = true);
void getContours(cv::Mat image, cv::Mat new_image, bool show_hull);