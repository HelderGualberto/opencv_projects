#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

std::vector<cv::Mat> read_images_from_file(char * data_names, const std::string prefix);

#endif