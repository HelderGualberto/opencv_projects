/*
This program is used to detect motorcycles on a designated area of the screen.
Click four times to designate the area of the screen to detect. Start with the
upper left and go clockwise. Then, drag the red line to indicate a prohibited 
zone to the left, and drag the blue line with the mouse to indicate a prohibited area to the right. Hit esc when finished after each drawing. Then drag lines to indicate lanes, if any. The program with run
automatically after pressing escape.

Once running, press q or esc to quit, or space to pause/unpause.

PARAMETERS / USAGE: all parameters optional. Defaults to not drawing lanes and prohibited areas.

./velocidade [0 = don't draw left, any other value draws] [draw right] [number of lanes to draw] [lane detection width in pixels] [distance of area drawn for velocity measurement]

*/

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>


const int KEY_SPACE = 32;
const int KEY_ESC = 27;
const int spd = 50;

const int CAR_MAX_WIDTH = 18;//X_SIZE / 9;
const int CAR_MAX_HEIGHT = 37;



int vectorial_distance(cv::Point2f p1, cv::Point2f p2){
	int dist = sqrt(pow(((p1.x - p2.x)),2) + pow(((p1.y - p2.y)),2));
	return dist;
}

int main(int argc, char **argv)
{
  std::cout << "Using OpenCV " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "." << CV_SUBMINOR_VERSION << std::endl;

  std::string filename = "";
  if(argc >1){
     filename = argv[1];
  }else
    return -1;


  cv::VideoCapture cap(filename);


/*
  cv::Point2f p1=cv::Point2f(587,240);
  cv::Point2f p2=cv::Point2f(446,265);
  cv::Point2f p3=cv::Point2f(494,393);
  cv::Point2f p4=cv::Point2f(672,322);

*/
  // Perspective transformation points
  cv::Point2f source_points[] = {p1,p2,p3,p4};
  
  int X_SIZE;//vectorial_distance(p1,p2);
  int Y_SIZE;

  if(vectorial_distance(p1,p2) > vectorial_distance(p4,p3)){
		X_SIZE = vectorial_distance(p1,p2);
    std::cout << X_SIZE  <<std::endl;

	}
	else{
		X_SIZE = vectorial_distance(p4,p3);
        std::cout << X_SIZE  <<std::endl;

	}

	if(vectorial_distance(p1,p4) > vectorial_distance(p2,p3)){
		Y_SIZE = vectorial_distance(p1,p4);
        std::cout << Y_SIZE  <<std::endl;

	}
	else{
		Y_SIZE = vectorial_distance(p2,p3);
    std::cout << Y_SIZE  <<std::endl;

	}
    int INIT_IMAGES = 50;

  /*cv::Point2f dst_points[] = {cv::Point2f(0,0), cv::Point2f(X_SIZE,0),cv::Point2f(X_SIZE,Y_SIZE),cv::Point2f(0,Y_SIZE)};

  cv::Mat transform_matrix = cv::getPerspectiveTransform(source_points,dst_points);
  cv::Mat inverse_matrix = cv::getPerspectiveTransform(dst_points,source_points);
  //std::cout << transform_matrix << std::endl;
  //std::cout << inverse_matrix << std::endl;
  //std::cout << transform_matrix.inv() << std::endl;
*/
  cv::Mat frame,dst, gray;
  
  cap >> frame;

  cv::cvtColor(frame,gray,CV_BGR2GRAY);

  cv::Mat average(frame.rows,frame.cols, CV_16UC3 ,1);
  cv::Mat gray_average(gray.rows,gray.cols, CV_16UC1 ,1);

  cv::imshow("Color Frame", frame);
  cv::imshow("Gray Frame", gray);



  for (int i=0; i<INIT_IMAGES; i++)
  {
    cap >> frame;
    
    if (frame.empty() || frame.rows == 0 || frame.cols == 0)
      break;
    
    cv::cvtColor(frame,gray,CV_BGR2GRAY);
    //cv::warpPerspective(dst,frame,transform_matrix,cv::Size(X_SIZE,Y_SIZE));
    frame.convertTo(frame, CV_16UC3);
    gray.convertTo(gray, CV_16UC1);

    cv::add(average, frame, average, cv::noArray(),-1);
    cv::add(gray_average, gray, gray_average, cv::noArray(),-1);
  }
    average = average/INIT_IMAGES;
    average.convertTo(average, CV_8UC3); 

    gray_average = gray_average/INIT_IMAGES;
    gray_average.convertTo(gray_average, CV_8UC1);

    cv::imshow("avg", average);
    cv::imshow("gacg", gray_average);

  while(cap.read(frame) & cv::waitKey(20) != KEY_SPACE){
    cv::cvtColor(frame,gray,CV_BGR2GRAY);
    cv::absdiff(average,frame,dst);
    cv::absdiff(gray_average,gray,gray);
    cv::imshow("bkg", dst);
    cv::imshow("gbkg", gray);
  }

    cv::waitKey(0);
  }