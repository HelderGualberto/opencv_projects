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
cv::Point2f get_center_rect(cv::Rect r) {
  return cv::Point2f((r.x + r.width / 2), (r.y + r.height / 2));
}

int main(int argc, char **argv)
{
  std::cout << "Using OpenCV " << CV_MAJOR_VERSION << "." << CV_MINOR_VERSION << "." << CV_SUBMINOR_VERSION << std::endl;

  std::ofstream file;
  file.open("analise.csv", std::ofstream::app);

  std::string filename = "";
  /*if(argc >1){
     filename = argv[1];
  }else
    return -1;
*/

  int X_SIZE = 400;//vectorial_distance(p1,p2);
  int Y_SIZE = 400;

  //cv::VideoCapture cap(filename);

  cv::Point2f p1=cv::Point2f(446,265);
  cv::Point2f p2=cv::Point2f(494,393);
  cv::Point2f p3=cv::Point2f(672,322);
  cv::Point2f p4=cv::Point2f(587,202);

/*
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
*/
  cv::Point2f dst_points[] = {cv::Point2f(0, 0), cv::Point2f(X_SIZE, 0),
                            cv::Point2f(X_SIZE, Y_SIZE),
                            cv::Point2f(0, Y_SIZE)};

  // Perspective transformation points
  cv::Point2f source_points[] = {p1,p2,p3,p4};
  


  cv::Mat org(1000,1000,CV_8UC1);
  cv::Mat dst(X_SIZE,Y_SIZE,CV_8UC1, 0), transform_matrix;

  transform_matrix = cv::getPerspectiveTransform(source_points, dst_points);

  cv::line(org, p1, p2, cv::Scalar(255,255,255), 1, 8,0);  
  cv::line(org, p2, p3, cv::Scalar(255,255,255), 1, 8,0);
  cv::line(org, p3, p4, cv::Scalar(255,255,255), 1, 8,0);
  cv::line(org, p4, p1, cv::Scalar(255,255,255), 1, 8,0);

  float x0 = p1.x + (p2.x-p1.x)/2;
  float x1 = p4.x + (p3.x-p4.x)/2;
  float y0 = p1.y + (p2.y-p1.y)/2;
  float y1 = p4.y + (p3.y-p4.y)/2;

  float m = (y1-y0)/(x1-x0);
  
  cv::Mat temp;

  std::vector<cv::Point2f > dst_pts;
  std::vector<cv::Point2f > src_pts;


  for(int x = x0+20; x<x1;x+=5){
    cv::Point2f pt(x, (m*(x-x0) + y0));
    src_pts.push_back(pt);

    cv::circle(org,pt,1, 255, 1, 8,0 );
    cv::imshow("org", org);
    cv::waitKey(0);
  }

  cv::perspectiveTransform(src_pts, dst_pts, transform_matrix);

  for(int i=0;i<dst_pts.size();i++){
    cv::circle(dst,dst_pts[i],1, 255, 1, 8,0 );
    std::cout << dst_pts[i] << std::endl;
   // cv::imshow("dst", dst);
   // cv::waitKey(0);
  }
  



  file.close();  
  }