
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

	cout << "Loading images..." << endl << endl;
	while(getline(names,filename)){
		filename = prefix + filename;
		cout << filename << endl;
		cv::Mat mat = cv::imread(filename,CV_LOAD_IMAGE_COLOR);
		imgs.push_back(mat);
	}

	names.close();

	cout << "Loaded images!" << endl;
	return imgs;
}

std::string int_to_string(int i){
	std::ostringstream s;
	s << i;
	std::string converted(s.str());
	return converted;
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
  	int c = 0;
  	vector<cv::Mat> imgs = read_images_from_file("../../data/dataset/data.names", "../../data/dataset/input/");

  	while(imgs.size() > 0){
  		cv::Mat mat = imgs.back();
  		correct_gamma(20.0, mat);
  		imgs.pop_back();
  		string path = "../../data/dataset/output/" + int_to_string(c) + ".jpg";
  		cv::imwrite(path, mat);
  		c++;
  	}

  return 0;
}

