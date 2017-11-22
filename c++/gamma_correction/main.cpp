
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>

//#include "../utils/utils.hpp"

using namespace std;

std::vector<cv::Mat> read_images_from_file(char * data_names, const string prefix){
	
	ifstream names(data_names);
	
	vector<cv::Mat> imgs;
	string filename;

	while(getline(names,filename)){
		filename = prefix + filename;
		cout << filename << endl;
		cv::Mat mat = cv::imread(filename,CV_LOAD_IMAGE_COLOR);
		imgs.push_back(mat);
	}

	names.close();

	return imgs;
}

void correct_gamma(float gamma, cv::Mat &mat){

	for(int i=0;i<mat.rows;i++){
		for(int j=0;j < mat.cols;j++){
			for(int c=0;c<3;c++){
				float normRGB =((float)mat.at<cv::Vec3b>(i,j)[c])/255.0;
				float gammaPower = pow(normRGB, (float)(1/gamma));
				uchar RGB = (uchar)(gammaPower*255);
				mat.at<cv::Vec3b>(i,j)[c] = RGB;
			}
		}
	}

}


int main(int argc, char ** argv)
{
  	
  	vector<cv::Mat> imgs = read_images_from_file("../../data/dataset/data.names", "../../data/dataset/predic/");

  	while(imgs.size() > 0){
  		cv::Mat mat = imgs.back();
  		cv::imshow("No gamma correction", mat);
  		correct_gamma(5.0, mat);
  		cv::imshow("Gamma 0.2", mat);
  		correct_gamma(2.2, mat);
  		cv::imshow("Gamma 0.3", mat);
  		cv::waitKey(5000);

  		imgs.pop_back();
  	}

  return 0;
}

