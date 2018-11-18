#ifndef ARMOR_DETECT_H
#define ARMOR_DETECT_H
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

/****************************************
* @class name  ArmorProcess
* @brief    装甲处理，
* @para     无
* @return   无
*****************************************/
class ArmorProcess
{
public:
   void ArmorDetect(void);
private:
   void DetectLightBar(Mat &frame);
};
#endif // ARMOR_DETECT_H
