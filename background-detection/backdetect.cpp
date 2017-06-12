#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <string>
#include <time.h>

using namespace cv;
using namespace std;
clock_t _time = 0;

//Ptr<BackgroundSubtractor> pMOG;

BackgroundSubtractorMOG2 *pMOG;

bool detectaM(Mat *mat){
	Mat thresh;
	threshold(*mat,thresh,127,255,0);
//	imshow("thresh",thresh);	

	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(thresh,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, Point(0,0));
		

	if(contours.size() < 1){
		return false;
	}
	int c=0;
	for(int i=0;i<contours.size();i++){
		Rect rect = boundingRect(contours[i]);
		cv::rectangle(*mat, rect, cv::Scalar(255,255,255), 2, 8, 0);

		int x = rect.x;
		int y = rect.y;
		int h = rect.height;
		int w = rect.width;
		
		int px = w/2.5;
		int py = h/2.5;
		x = int(x-px);
		y = int(y - py);
		w += int(2*px);
		h += int(2*py);

		if(h*w > 25){
			
			cout << h*w << endl;
			return true;	
		}		
	}

	return false;
}

void process(Mat &mat){

	Mat reduzido;
	float fatorg = 0.125;
	resize(mat,reduzido,Size(0,0),fatorg,fatorg,INTER_LINEAR);	
	Mat fgMask(reduzido.rows,reduzido.cols,CV_8UC3);
	
	pMOG->operator()(reduzido,fgMask);
	//cout << fgMask->type() << endl;
	imshow("fg",fgMask);
	//imshow("reduzido",reduzido);



	if(detectaM(&fgMask)){
	//	putText(mat,"MOVENDO",Point(0,mat.rows),FONT_HERSHEY_SIMPLEX,10,Scalar(0,0,255),2,8,false);
//		cout << "MOVENDO" << endl;	
	}
}


int main(int argc, char* argv[])
{
    
    
//	VideoCapture cap("rtsp://admin:B30cd4Ro@192.168.10.180:554/LiveMedia/ch1/Media2");
//	VideoCapture cap("rtsp://adm-helder:fullsec123@192.168.10.181:554/H264");
//	VideoCapture cap("http://192.168.10.234:8090/channel2/video.ts");
//	VideoCapture cap(0);
	VideoCapture cap("../data/face2.webm");
	pMOG = new BackgroundSubtractorMOG2();

	if (!cap.isOpened())  // if not success, exit program
	{
        	cout << "Cannot open the video cam" << endl;
		return -1;
	}
	Mat frame;
	while(true){	
		bool bSuccess = cap.read(frame); 
        if (!bSuccess) //if not success, break loop
        	{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}

       	// read a new frame from video
		process(frame);
		//clock_t now = clock();

		//float dif = ((float)(now - _time)/CLOCKS_PER_SEC);
		
		//_time = clock();
		//float rate = 1/dif;
		//cout << rate<< endl;
		
		
//		imshow("MyVideo", frame); //shows the frame in "MyVideo" window
		waitKey(100); //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	}

	return 0;
}
