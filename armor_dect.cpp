#include <opencv2/opencv.hpp>
#include <iostream>
#include "math.h"

#define uint unsigned int

//#define  RED
#define BLUE
#define BRIGHT_THRESHOLD  220 
#define COLOR_THRESHOLD   45


using namespace std;
using namespace cv;
Mat frame;

std::vector<RotatedRect> light_bar;


Mat get_frame(VideoCapture cap);
void quit_debug(void);
std::vector<RotatedRect>  find_light_bar(Mat& src_image);
void draw_rotatedrect(cv::Mat &img, const cv::RotatedRect &rect, const cv::Scalar &color, int thickness);
void light_bar_filter(std::vector<RotatedRect> &light,Mat &filter_image);
void adjust_rect(cv::RotatedRect &rect);
int main(int argc,char** argv)
{
	
	VideoCapture capture(0);
	//if(!capture.isOpend()) {
	//	return -1;
	//}
    while(1) {
    	Mat src_image;
    	src_image = get_frame(capture);
    	light_bar = find_light_bar(src_image);
    	Mat lights_bar_after_filter = cv::Mat::zeros(src_image.size(), CV_8UC3); 
    	light_bar_filter(light_bar,lights_bar_after_filter);
        quit_debug();	
    }
    
    return 0;
}
/**
 * @ get source frame
 * @param   VideoCapture
 * @return  Mat 
 */
Mat get_frame(VideoCapture cap) 
{   
	Mat frame;
	//cap >> frame;
	do
	{
    	cap>>frame;
	}while(frame.empty());
	return frame;
}
/**
 * @ quit the debug mode
 */
void quit_debug()
{
	if(waitKey(1) == 'q') {
      	exit(0);
    }
}
void draw_rotatedrect(cv::Mat &img, const cv::RotatedRect &rect, const cv::Scalar &color, int thickness) 
{    
		cv::Point2f vertex[4];
		rect.points(vertex);
		for (int i = 0; i < 4; i++)
			cv::line(img, vertex[i], vertex[(i + 1) % 4], color, thickness);

}
/**
 * @Author    percy
 * @copyright [copyright]
 * @license   [license]
 * @version   [version]
 * @param
 */
std::vector<RotatedRect> find_light_bar(Mat& src_image)
{
	Mat gray_image;
	Mat sub_colors;
	Mat brightness_iamge;
	Mat color_image;
	Mat light_bar_image;
	Mat lights_bar_before_filter = cv::Mat::zeros(src_image.size(), CV_8UC3);
	 
	std::vector<Mat> BGR_channels;
	std::vector<RotatedRect> light_bar;
	std::vector<std::vector<Point>> light_bar_contours;
	std::vector<std::vector<Point>> brightness_contours;
	//cvtColor(src_image,gray_image,CV_BGR2HSV);
	cvtColor(src_image,gray_image,CV_BGR2GRAY); 
    split(src_image,BGR_channels); //颜色通道分离
#ifdef BLUE 
    //subtract(BGR_channels[0],BGR_channels[1],sub_colors);// B - G
    subtract(BGR_channels[0],BGR_channels[1],sub_colors);// B - G
#else
    subtract(BGR_channels[2],BGR_channels[1],sub_colors);// R - G
#endif
    blur(gray_image,gray_image,Size(3,3));
    //blur(sub_colors,sub_colors,Size(3,3));
    Mat element = getStructuringElement(MORPH_RECT,Size(5,5));

    threshold(gray_image,brightness_iamge,BRIGHT_THRESHOLD,255,THRESH_BINARY);
    threshold(sub_colors,color_image,COLOR_THRESHOLD,255,THRESH_BINARY);
    dilate(color_image,color_image,element,Point(-1,-1),1);
    dilate(brightness_iamge,brightness_iamge,element,Point(-1,-1),1);
    light_bar_image = color_image & brightness_iamge;
    //dilate(light_bar_image,light_bar_image,element,Point(-1,-1),1);	
   //find contours 
  
   Scalar color(0,255,0);
   findContours(light_bar_image,light_bar_contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
   findContours(brightness_iamge,brightness_contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
   //drawContours(light_bar_image,light_bar_contours,0,color);
   //drawContours(brightness_iamge,brightness_contours,0,color);
   light_bar.reserve(brightness_contours.size());
   std::vector<int> process_flag(brightness_contours.size());
   for( uint i = 0; i < light_bar_contours.size();++i) {
   		for(uint j = 0; j < brightness_contours.size();++j) {
   			if(pointPolygonTest(brightness_contours[j],light_bar_contours[i][0],true) >= 0.0) {
   				RotatedRect temp_light_bar = minAreaRect(brightness_contours[j]);
   				light_bar.push_back(temp_light_bar);
   				draw_rotatedrect(lights_bar_before_filter, temp_light_bar,Scalar(0,0,255), 1);
   				process_flag[j] = true;
   				break;
   			}
   		}
   }
   
   //imshow("srcimage",src_image);
   //imshow("gray_image",gray_image);
   //imshow("sub_colors",sub_colors);
   imshow("color_image",color_image);
   imshow("brightness_iamge",brightness_iamge);
   imshow("light_bar_image",light_bar_image);
   imshow("lights_bar_before_filter",lights_bar_before_filter);
   return  light_bar;
}
/**
 * @Author    percy
 * @copyright [copyright]
 * @license   [license]
 * @version   [version]
 * @param     light       [description]
 */
void light_bar_filter(std::vector<RotatedRect> &lights,Mat &filter_image)
{
	
	 std::vector<cv::RotatedRect> light_bar_rects ;
	 for(uint i = 0; i < lights.size();i++) {
	 	 adjust_rect(lights[i]);//
	 }
	 for(const auto &armor_light : lights) {
	 	auto angle = armor_light.angle;
	 	auto rect = std::minmax(armor_light.size.width,armor_light.size.height);
	 	auto  light_bar_ratio = rect.second / rect.first;
	 	 /*
	 	{
	 		light_bar_rects.push_back(armor_light);
	 		draw_rotatedrect(filter_image,armor_light,Scalar(255,0,0),1);
	 	}
	 	 */
	 }	
	 //imshow("lights_bar_after_filter",filter_image);
	 //imshow("lights_bar_after_filter",lights);
	 lights = light_bar_rects;
}
//void matched_armor



/**
 * @Author    percy
 * @copyright [copyright]
 * @license   [license]
 * @version   [version]
 * @param     rect        [description]
 */
void adjust_rect(cv::RotatedRect &rect)
{
	if(rect.size.width > rect.size.height) {
		auto temp = rect.size.height;
		rect.size.height = rect.size.width;
		rect.size.width = temp;
		rect.angle += 90;
	}
}


