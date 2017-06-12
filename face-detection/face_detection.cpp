#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <string>
#include <time.h>
#include <sstream>
#include <climits>

using namespace std;


cv::BackgroundSubtractorMOG2 *pMOG;
cv::CascadeClassifier cascade;
cv::Scalar colors[20] = {
	cv::Scalar(0,0,255),
	cv::Scalar(0,255,0),
	cv::Scalar(255,0,0),
	cv::Scalar(255,255,255),
	cv::Scalar(255,0,255),
	cv::Scalar(255,255,0),
	cv::Scalar(0,255,255),
	cv::Scalar(127,255,0),
	cv::Scalar(127,127,127),
	cv::Scalar(255,127,0),
	cv::Scalar(0,0,255),
	cv::Scalar(0,255,0),
	cv::Scalar(255,0,0),
	cv::Scalar(255,255,255),
	cv::Scalar(255,0,255),
	cv::Scalar(255,255,0),
	cv::Scalar(0,255,255),
	cv::Scalar(127,255,0),
	cv::Scalar(127,127,127),
	cv::Scalar(255,127,0)
};

int faces_detected = 0;

//Reduction size factor to compute move detection
float fatorg = 0.4;

//	cv::VideoCapture cap("rtsp://admin:huawei123@10.1.1.0:554/LiveMedia/ch1/Media1");

//--------------------------------------*********--------------------------------------------
//	cv::VideoCapture cap("rtsp://viwer:foscam123@10.1.1.5:554/videoMain");
//--------------------------------------make*********--------------------------------------------
	cv::VideoCapture cap("../data/face.webm");
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
	cv::VideoCapture cap(video);
*/
//--------------------------------------*********--------------------------------------------


void draw_rect(cv::Mat &frame, cv::Rect rect){
	cv::rectangle(frame, rect, cv::Scalar(0,0,255), 1, 8, 0);
}

//Return the geometric center of a rect.
cv::Rect square_geometric_center(cv::Rect rect){
	int x = rect.x+rect.width/2;
	int y = rect.y+rect.height/2;

	cv::Rect gc_point(x,y,1,1);
	return gc_point;
}

//This function will make an and operation src(i,j)*src2(i,j) > 0 -> true

cv::Mat and_operation(cv::Mat &src1, cv::Mat &src2){
	
	int w,h;

	if(src1.rows > src2.rows){
		h = src2.rows-1;
	}
	else
		h = src1.rows-1;

	if(src1.cols > src2.cols){
		w = src2.cols-1;
	}
	else
		w = src1.cols-1;

	cv::Mat *out = new cv::Mat(h,w,CV_8UC1);

	for(int i=0;i<h;i++){
		for(int j=0;j<w;j++){
			if(src1.at<uchar>(i,j)*src2.at<uchar>(i,j) > 0)
				out->at<uchar>(i,j) = 255;
			else
				out->at<uchar>(i,j) = 0;	
		}
	}

	return *out;
}

cv::Mat apply_binary_threshold(cv::Mat &src){
	cv::Mat *out = new cv::Mat(src.rows+1,src.cols+1,CV_8UC1,cv::Scalar(0));

	for(int i=0;i<src.cols;i++){
		for(int j=0;j<src.rows;j++){

			if(src.at<int>(j,i) > 2500){
				out->at<uchar>(j,i) = 255;

			}else{
				out->at<uchar>(j,i) = 0;
			}
		}
	}
	return *out;
}

template <typename T>
  std::string NumberToString ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

void save_img(cv::Mat frame, int img_number){
	std::string fd = NumberToString(img_number);
	std::string path = "./img/face_" + fd + ".jpg";
	cv::imwrite(path,frame);
}

void crop_img(cv::Mat &src, cv::Mat& dst, cv::Rect rect){
	dst = src(rect);
}

cv::Rect resize_rect(cv::Rect rect, float sf){
    int h = rect.height/sf;
    int w = rect.width/sf;
    int x = rect.x/sf;
    int y = rect.y/sf;
    
    cv::Rect *r = new cv::Rect(x,y,w,h);
    
    return *r;
}

void erode_img(cv::Mat &src, cv::Mat &dst){
	int erosion_size = 0;
	int erosion_type = cv::MORPH_ELLIPSE;

	cv::Mat element;

	cv::erode(src, dst, element, cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}

void dilate_img(cv::Mat &src, cv::Mat &dst){
	int erosion_size = 5;
	int erosion_type = cv::MORPH_RECT;

	cv::Mat element;

	cv::dilate(src, dst, element, cv::Point(-1,-1), 5, cv::BORDER_CONSTANT, cv::morphologyDefaultBorderValue());
}


cv::Rect get_largest_move(cv::Mat &res, cv::Mat& org){
	
	vector< vector<cv::Point> > contours;
	vector<cv::Vec4i> hierarchy;

	cv::findContours(res,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE, cv::Point(0,0));
	
	int ref=0;
	int area = 0;
	if(contours.size() < 1){
		return cv::Rect(0,0,0,0);
	}
	

	for(int i=0;i<contours.size();i++){
		cv::Rect rect = cv::boundingRect(contours[i]);
		if(rect.width*rect.height > area){
			area = rect.width*rect.height;
			ref=i;
		}
	}

	cv::Rect r;
	r = resize_rect(cv::boundingRect(contours[ref]), fatorg);
	cv::rectangle(org, r, cv::Scalar(255,255,255), 2, 8, 0);

	return r;
}

void detect_move_v2(cv::Mat &mat){
	cv::Mat reduzido, erode, dilate;
	
	//Resize the image by the fatorg
	cv::resize(mat,reduzido,cv::Size(0,0),fatorg,fatorg,cv::INTER_LINEAR);
	
	//Create an image to put the foregroud detection
	cv::Mat fgMask(reduzido.rows,reduzido.cols,CV_8UC1);

	//Get the foreground subtraction and put it into fgMask
	pMOG->operator()(reduzido,fgMask);

	//Erode the image from fgMask to eliminate high frequency noise
	erode_img(fgMask,erode);


	dilate_img(erode, dilate);

	get_largest_move(dilate, mat);

	//imshow("erode",dilate);
	//Integrate the eroded image to get the movement location
}


bool detect_move(cv::Mat &mat, cv::Mat &out){

	cv::Mat reduzido, reduzido_F, erode, integral_img, binary, binary_F, and_img;
	
	//Resize the image by the fatorg
	cv::resize(mat,reduzido,cv::Size(0,0),fatorg,fatorg,cv::INTER_LINEAR);
	
	//Create an image to put the foregroud detection
	cv::Mat fgMask(reduzido.rows,reduzido.cols,CV_8UC1);

	//Get the foreground subtraction and put it into fgMask
	pMOG->operator()(reduzido,fgMask);

	//Erode the image from fgMask to eliminate high frequency noise
	erode_img(fgMask,erode);
	
	//Integrate the eroded image to get the movement location
	cv::integral(erode, integral_img, CV_32S);
	
	//Apply a manual binary threshold over a CV_32SC1 and return into CV_8UC1
	binary = apply_binary_threshold(integral_img);
	//imshow("binary",binary);
	//Repeat all the previous processes for the flip image

	//Flip mat around y axis
	cv::flip(reduzido, reduzido_F, 1);

	//Create an image to put the foregroud detection
	cv::Mat fgMask_F(reduzido_F.rows,reduzido_F.cols,CV_8UC1);

	//Get the foreground subtraction and put it into fgMask
	pMOG->operator()(reduzido_F,fgMask_F);

	//Erode the image from fgMask to eliminate high frequency noise
	erode_img(fgMask_F,erode);

	//Integrate the eroded image to get the movement location
	cv::integral(erode, integral_img,CV_32S);
	
	//Apply a manual binary threshold over a CV_32SC1 and return into CV_8UC1
	binary_F = apply_binary_threshold(integral_img);
	
	//Reflip the binary_F
	cv::Mat binary_RF;
	cv::flip(binary_F,binary_RF,1);

	//Make the 'and' operation over binary and binary_RF
	and_img = and_operation(binary_RF,binary);//cv::bitwise_and(binary_RF,binary,and_img);
	//imshow("and",and_img);
	cv::Rect move_rect = get_largest_move(and_img, mat);
	crop_img(mat, out,move_rect);

	if(move_rect.width*move_rect.height < 2500){
		return false;
	}

	return true;
}

bool detect_face(cv::Mat &mat){
	cv::Mat input_mat = mat.clone();    

	std::vector<cv::Rect> faces;
	cv::Mat gray_mat;

	cv::cvtColor(mat,gray_mat,CV_BGR2GRAY);

	cascade.detectMultiScale(gray_mat, faces, 1.3, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(50,50));
	if(faces.size() > 0){
		
		
		for(unsigned int i = 0; i < faces.size();i ++){
			cd:string s = "Person " + NumberToString(i);
			//cv::Rect r = resize_rect(faces[i],sf);
			cv::rectangle(input_mat, faces[i], colors[i], 2, 8, 0);
			cv::putText(input_mat, s, cv::Point(faces[i].x,faces[i].y-2),cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(255,255,255) ,1.5,5,false);
		}

		input_mat.copyTo(mat);

		//save_img(input_mat, faces_detected);

		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
    cout << "Cascade Classifier. v1.2" << endl;

	string cascade_location = "../haarcascades/haarcascade_frontalface_alt.xml";	
	cascade.load(cascade_location);	
	pMOG = new cv::BackgroundSubtractorMOG2();

	if (!cap.isOpened())  // if not success, exit program
	{
        	cout << "Cannot open the video cam" << endl;
		return -1;
	}

	cout << "Camera resolution: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << " x " << cap.get(CV_CAP_PROP_FRAME_HEIGHT)<<endl;

	int FRAME_REF = 50;	
	int frame_counter = 0;
	
	float avg_process_time = 0;

	cv::Mat frame;
	
	clock_t p1,p2;

	time_t start;
	time(&start);

	for(;;){

		if(!cap.read(frame)){
			cout << "End of file" << endl;
			break;
		}

		
		p1 = clock();

/*
		if(detect_face(frame)){
			faces_detected++;
		}
		cv::imshow("frame",frame);
*/

		cv::Mat crop;
		bool move = detect_move(frame,crop);

		if(move){
			
			if(detect_face(crop)){
				faces_detected++;
			}
			imshow("Crop", crop);
		}

		//Mat crop;
		//crop_img(frame,crop,cv::Rect(0,0,0.65*frame.cols,frame.rows));


		p2 = clock(); 		
 		

 		avg_process_time += ((float)(p2-p1)/CLOCKS_PER_SEC);
	
		//imshow("Crop", crop);		
		//imshow("Frame", frame);


		char k;

		k = cv::waitKey(1);

		if(k == 's'){
			break;
		}

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
