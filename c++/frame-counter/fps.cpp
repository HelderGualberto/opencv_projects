#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <string>
#include <time.h>
#include <ctime>

using namespace cv;
using namespace std;
int main(int argc, char* argv[])
{
    
	//VideoCapture cap("rtsp://admin:B30cd4Ro@192.168.10.180:554/LiveMedia/ch1/Media1");
	VideoCapture cap("rtsp://adm-helder:fullsec123@192.168.10.181:554/H264");
//	VideoCapture cap("http://192.168.10.234:8090/channel2/video.ts");
//	VideoCapture cap(0);
//	VideoCapture cap("/home/user/opencv/corredor.mp4");
	double start = (double)time(0);
	cout << "Start: " << start << endl;
	Mat frame;
	for(int i =0;i<100;i++){	
		bool bSuccess = cap.read(frame); // read a new frame from video
	}
	double end = (double)time(0);
	cout << "End: " << end << endl;
	double fps = 1/((end-start)/100);
	cout << "Avg FPS: " << fps << endl;
	return 0;
}
