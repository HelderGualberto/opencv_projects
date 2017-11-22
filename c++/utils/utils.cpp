#include "utils.hpp"

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