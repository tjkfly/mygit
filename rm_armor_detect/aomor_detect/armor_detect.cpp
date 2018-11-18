#include "armor_detect.h"

/****************************************
* @funcName ArmorDetect
* @brief    装甲检测
* @para     无
* @return   无
*****************************************/
void ArmorProcess::ArmorDetect()
{
    VideoCapture cap(0);
    if(!cap.isOpened()){
        cout<<"打开摄像头失败！！！"<<endl;
        exit(0);
    }
    while(1) {
        Mat src_image;
        do {
            cap >> src_image;
        }while(src_image.empty());
        DetectLightBar(src_image);
        imshow("原图",src_image);
        if(waitKey(20) =='q') {
            break;
        }
    }
}
/****************************************
* @funcName DetectLightBar
* @brief    检测装甲灯条
* @para     Mat 原始图像 ,
* @return   RotatedRect 灯条旋转矩形向量
*****************************************/
void ArmorProcess::DetectLightBar(Mat &frame)
{
    Mat gray_image;
    cvtColor(frame,gray_image,CV_BGR2GRAY);
    imshow("灰度图",gray_image);
}





































