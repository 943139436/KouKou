#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H
#include<opencv2/opencv.hpp>
#include<QPoint>
#include<cmath>

using namespace cv;
class imageProcessing{

public:
    imageProcessing();

     static Mat bgImage;//背景图像
     static std::string imgpath;
     static std::string imgOutpath;//输入和输出路径
     static Mat inputImage;//输入图像，用于openCV处理
     static Mat outputImage;//输出图像
     static QPoint point_0;//矩形框左上角
     static QPoint point_1;//矩形框右下角

   static Mat changeBackground(Mat inputImg,Mat bgImg,QPoint point_0,QPoint point_1);//图像处理函数
};
#endif // IMAGEPROCESSING_H
