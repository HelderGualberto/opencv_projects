
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>


using namespace std;

int main(int argc, char ** argv)
{
  	

	cv::VideoCapture cap = new cv::VideoCapture("../../data/face.webm");

	cv::Mat frame;

	while(cap.read(frame)){

		cv::waitKey(30);

		cv::imshow("Img", frame);

	}



  return 0;
}

