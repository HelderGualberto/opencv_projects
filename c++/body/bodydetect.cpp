#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <time.h>
#include <sstream>

using namespace cv;
using namespace std;

void draw_rect(Mat &frame, Rect rect){
	rectangle(frame, rect, Scalar(0,0,255), 1, 8, 0);
}

cv::Rect resize_rect(cv::Rect rect, float sf){
    
	
	int h = rect.height/sf;
	int w = rect.width/sf;
	int x = rect.x/sf;
	int y = rect.y/sf;
	
	cv::Rect *r = new cv::Rect(x,y,w,h);	

	return *r;
}

int main(int argc, char* argv[])
{
    
    	cout << "tesets " << 1.1 << endl;
//	VideoCapture cap("rtsp://admin:B30cd4Ro@192.168.10.180:554/LiveMedia/ch1/Media2");
//	VideoCapture cap("rtsp://adm-helder:fullsec123@192.168.10.181:554/H264");
	VideoCapture cap("../move.webm");
//	VideoCapture cap("https://d1h84if288zv9w.cloudfront.net/aovivo/0be3_907.stream/chunklist.m3u8");

	HOGDescriptor hog;	
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());


	if (!cap.isOpened())  // if not success, exit program
	{
        	cout << "Cannot open the video cam" << endl;
		return -1;
	}
	Mat frame;
	Mat gray_frame;
	Mat resized_frame;
	Mat hog_frame;

	int hog_detect = 0;
	


	cap.read(frame);
	
	cout << "Rows: " << frame.rows << endl;
        cout << "Cols: " << frame.cols << endl;

	float sf = float(1280.0/frame.cols);
	

	std::vector<Rect> hog_rect;

	while(true){
        	bool bSuccess = cap.read(frame); // read a new frame from video
	
		if (!bSuccess)
                {
                        
			cout << "End of stream" << endl;
			cap.open("../move.webm");
			cout << "Rows: " << resized_frame.rows << endl;
	                cout << "Cols: " << resized_frame.cols << endl;
			cout << "HOG: " << hog_detect << endl;
			cout << "X-----------------X" << endl;
			hog_detect = 0;
			
			break;
                }

		resize(frame, resized_frame, Size(0,0), sf, sf, INTER_NEAREST);

		cv::Rect rect2(cv::Point(100, 100),cv::Size(50,50));
		
		draw_rect(resized_frame, rect2);
// OBS. Se estiver utilizando a camera da huawei, descomentar as linhas abaixo

		transpose(resized_frame, hog_frame);
		flip(hog_frame, resized_frame, 0);
		
		cvtColor(resized_frame,gray_frame, CV_BGR2GRAY);
		equalizeHist(gray_frame, gray_frame);		
		
		hog.detectMultiScale(gray_frame, hog_rect, 0, Size(8,8), Size(32,32), 1.1, 2);

		if(hog_rect.size() > 0){
                        for(int i = 0; i<hog_rect.size(); i++){
                                draw_rect(resized_frame, hog_rect[i]);
                        }
			hog_detect += hog_rect.size();
                }
		
		imshow("Window", resized_frame);

		waitKey(66);
	}

	return 0;
}
