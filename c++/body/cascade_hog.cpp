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

int main(int argc, char* argv[])
{
    
    	cout << "tesets " << 1.1 << endl;
//	VideoCapture cap("rtsp://admin:B30cd4Ro@192.168.10.180:554/LiveMedia/ch1/Media2");
//	VideoCapture cap("rtsp://adm-helder:fullsec123@192.168.10.181:554/H264");
	VideoCapture cap("../move.webm");
	//VideoCapture cap("https://d1h84if288zv9w.cloudfront.net/aovivo/0be3_501.stream/chunklist.m3u8");

	string cascade_location = "/usr/share/opencv/haarcascades/haarcascade_upperbody.xml";	
	CascadeClassifier body_cascade;
	HOGDescriptor hog;	


	body_cascade.load(cascade_location);	
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

	int cascade_detect = 0;
	int hog_detect = 0;

	float threshold = 2;
	float scale = 1.1;
	cap.read(frame);
	
	float size_factor = 0.2;

	std::vector<Rect> bodys_rect, hog_rect;

	
	for(int i = 0; i < 10; i++){	
		while(true){
        	bool bSuccess = cap.read(frame); // read a new frame from video
	
		if (!bSuccess)
                {
                        cout << "End of stream" << endl;
			cap.open("../move.webm");
			cout << "Threashold: "  << threshold << endl;
			cout << "Scale: " << scale << endl;
			cout << "HOG: " << hog_detect << endl;
			cout << "Cascade: "  << cascade_detect << endl;
			cout << "X-----------------X" << endl;
			cascade_detect = 0;
			hog_detect = 0;
			break;
                }

		resize(frame, resized_frame, Size(0,0), size_factor, size_factor, INTER_NEAREST);
		transpose(resized_frame, hog_frame);
		flip(hog_frame, resized_frame, 0);
		hog_frame = resized_frame.clone();
		
		cvtColor(resized_frame,gray_frame, CV_BGR2GRAY);
		equalizeHist(gray_frame, gray_frame);		
		
		body_cascade.detectMultiScale(gray_frame, bodys_rect, scale, threshold, 0, Size(30,30));
		hog.detectMultiScale(gray_frame, hog_rect, 0, Size(8,8), Size(32,32), scale, threshold);

		if(bodys_rect.size() > 0){
			for(int i = 0; i<bodys_rect.size(); i++){
				draw_rect(resized_frame, bodys_rect[i]);
			}
			cascade_detect += bodys_rect.size();
		}
		if(hog_rect.size() > 0){
                        for(int i = 0; i<hog_rect.size(); i++){
                                draw_rect(hog_frame, hog_rect[i]);
                        }
			hog_detect += hog_rect.size();
                }
		
		imshow("cascade", resized_frame);
		imshow("hog", hog_frame);

		waitKey(1);
		}
	}

	return 0;
}
