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

bool detectaM(Mat *mat,int sx,int sy){
	Mat somaf;
	cv::integral(*mat,somaf,CV_64F);
//	cv::imshow("mascara",somaf);
	int h = mat->rows;
	int w = mat->cols;
	
	float px = w/sx;
	float py = h/sy;
	
	float bmax = (float)(w*h/(sx*sy))*255.0;
	
	for(int ix=0;ix < sx;ix++){
		for(int iy = 0;iy < sy;iy++){
			float x1 = ix*px;
			float y1 = iy*py;
			float x2 = (ix+1)*px - 1;
			float y2 = (iy + 1) * py -1;
			float v = somaf.at<double>(y2,x2)-somaf.at<double>(y2,x1)-somaf.at<double>(y1,x2)+somaf.at<double>(y1,x1);
		//	cout << somaf.at<float>(y2,x2) << " " << somaf.at<float>(y2,x1) << " " <<somaf.at<float>(y1,x2) << " " << somaf.at<float>(y1,x1) << endl;
			//d::cout << v << std::endl;
			float pp = v/bmax;
			if(pp > 0.1)
				return true;
		}
	}

	return false;
}

void process(Mat &mat){

	Mat reduzido;
	float fatorg = 0.5;
	resize(mat,reduzido,Size(0,0),fatorg,fatorg,INTER_LINEAR);	
	Mat fgMask(reduzido.rows,reduzido.cols,CV_8UC3);
	
	pMOG->operator()(reduzido,fgMask);
	//cout << fgMask->type() << endl;
	//imshow("fg",fgMask);
	//imshow("reduzido",reduzido);

	if(detectaM(&fgMask,16,6)){
	//	putText(mat,"MOVENDO",Point(0,mat.rows),FONT_HERSHEY_SIMPLEX,10,Scalar(0,0,255),2,8,false);
		//cout << "MOVENDO" << endl;	
	}
}


int main(int argc, char* argv[])
{
    
    
	//VideoCapture cap("rtsp://admin:B30cd4Ro@192.168.10.180:554/LiveMedia/ch1/Media1");
//	VideoCapture cap("rtsp://adm-helder:fullsec123@192.168.10.181:554/H264");
//	VideoCapture cap("http://192.168.10.234:8090/channel2/video.ts");
//	VideoCapture cap(0);
	VideoCapture cap("https://d1h84if288zv9w.cloudfront.net/aovivo/0be3_69.stream/playlist.m3u8");

	pMOG = new BackgroundSubtractorMOG2();

	if (!cap.isOpened())  // if not success, exit program
	{
        	cout << "Cannot open the video cam" << endl;
		return -1;
	}
	Mat frame;
	int i=0;
	while(true){	
        	bool bSuccess = cap.read(frame); // read a new frame from video
		//process(frame);
		
		clock_t now = clock();
		//putText(frame,std::to_string(i),Point(0,frame.rows),FONT_HERSHEY_SIMPLEX,10,Scalar(0,0,255),2,8,false);
		imshow("teste",frame);
		i++;
		float dif = ((float)(now - _time)/CLOCKS_PER_SEC);
		
		_time = clock();
		float rate = 1/dif;
		cout << rate<< endl;
	//Mat* pFrame = process(frame);
		
//		imshow("MyVideo", frame); //shows the frame in "MyVideo" window
		waitKey(1);
	}

	return 0;
}
