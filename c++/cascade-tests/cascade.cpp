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

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

void save_img(Mat frame, int img_number){
	std::string fd = NumberToString(img_number);
	std::string path = "./img/face_" + fd + ".jpg";
	imwrite(path,frame);
}


cv::Rect resize_rect(cv::Rect rect, float sf){
    int h = rect.height/sf;
    int w = rect.width/sf;
    int x = rect.x/sf;
    int y = rect.y/sf;
    
    cv::Rect *r = new cv::Rect(x,y,w,h);
    
    return *r;
}


/*
void process(){

	Mat gray_frame;
	Mat resized_frame;
	Mat hog_frame;

	float threshold = 2;
	float scale = 1.1;
	cap.read(frame);
	
	float size_factor = 1.0;//(float)(640.0/frame.cols);
	
	int cascade_detect = 0;

	std::vector<Rect> rects;

	for(int i = 0; i < 10; i++){	
		while(true){
        	bool bSuccess = cap.read(frame); // read a new frame from video
	
		if (!bSuccess)
                {
                        cout << "End of stream" << endl;
			cap.open("../move.webm");
			cout << "Threashold: "  << threshold << endl;
			cout << "Scale: " << scale << endl;
			cout << "Detected rects: "  << cascade_detect << endl;
			cout << "X-----------------X" << endl;
			cascade_detect = 0;
			
			break;
                }

		resize(frame, resized_frame, Size(0,0), size_factor, size_factor, INTER_NEAREST);
		//transpose(resized_frame, hog_frame);
		//flip(hog_frame, resized_frame, 0);
		hog_frame = resized_frame.clone();
		
		cvtColor(resized_frame,gray_frame, CV_BGR2GRAY);
		equalizeHist(gray_frame, gray_frame);		
		
		cascade.detectMultiScale(gray_frame, rects, scale, threshold, 0, Size(30,30));

		if(rects.size() > 0){
			for(int i = 0; i<rects.size(); i++){
				draw_rect(resized_frame, rects[i]);
			}
			cascade_detect += rects.size();
		}
		
		imshow("cascade", resized_frame);

		waitKey(66);
		}
	}
}*/

bool detect_face(Mat &mat, CascadeClassifier face_cascade){
	cv::Mat input_mat = mat.clone();    

	std::vector<cv::Rect> faces;
	cv::Mat gray_mat;
	// C++: void resize(InputArray src, OutputArray dst, Size dsize, double fx=0, double fy=0, int interpolation=INTER_LINEAR )
	Mat resized;
	float sf = 0.5;

	cv::resize(mat, resized, cv::Size(0,0),sf,sf, INTER_NEAREST);
	
	cv::cvtColor(resized,gray_mat,CV_BGR2GRAY);

	face_cascade.detectMultiScale(gray_mat, faces, 1.2, 3, 0|CV_HAAR_SCALE_IMAGE, cv::Size(75,75));

	if(faces.size() > 0){
		for(unsigned int i = 0; i < faces.size();i ++){
			cv::Rect r = resize_rect(faces[i],sf);
			cv::rectangle(input_mat, r, cv::Scalar(255,0,0), 2, 8, 0);
		}

		input_mat.copyTo(mat);

		
		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
    cout << "Cascade Classifier. v1.2" << endl;

//	VideoCapture cap("rtsp://admin:huawei123@192.168.10.180:554/LiveMedia/ch1/Media1");

//--------------------------------------*********--------------------------------------------
//	VideoCapture cap("rtsp://adm-helder:fullsec123@192.168.10.181:554/H264");
//--------------------------------------*********--------------------------------------------
	VideoCapture cap("../data/face2.webm");
//--------------------------------------*********--------------------------------------------
/*
	char *video;
	
	if(argc > 1){
		video = argv[1];
		cout << video << endl;
	}else{
		cout << "Missing Cam URL Argument..." << endl;
		return 0;
	}
	VideoCapture cap(video);
*/
//--------------------------------------*********--------------------------------------------

	string cascade_location = "../../opencv-2.4.12/data/haarcascades/haarcascade_frontalface_alt.xml";	
	CascadeClassifier cascade;

	cascade.load(cascade_location);	

	if (!cap.isOpened())  // if not success, exit program
	{
        	cout << "Cannot open the video cam" << endl;
		return -1;
	}


	int FRAME_REF = 50;	
	int frame_counter = 0;
	int faces_detected = 0;
	float avg_process_time = 0;

	Mat frame;
	
	clock_t p1,p2;

	time_t start;
	time(&start);

	for(;;){

		if(!cap.read(frame)){
			cout << "End of file" << endl;
			break;
		}

		p1 = clock();
		if(detect_face(frame,cascade)){
			
			faces_detected++;
		}
 		p2 = clock();

 		avg_process_time += ((float)(p2-p1)/CLOCKS_PER_SEC);
	
		imshow("Window", frame);
		waitKey(1);
		frame_counter++;
	}

	time_t end;
	time(&end);
	float diff = difftime(end,start);

	avg_process_time = avg_process_time/frame_counter;
	cout << "Average frame rate: " << frame_counter/diff << endl;
	cout << "Faces detected: " << faces_detected << endl;
	cout << "Average process time: " << avg_process_time << endl;
	
	return 0;
}
