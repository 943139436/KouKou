#include "mainwindow.h"
#include<imageprocessing.h>
#include<showlabel.h>
#include <QApplication>

Mat imageProcessing::bgImage = Mat(Size(1920,1080),CV_8UC3,Scalar(200,200,200));
std::string imageProcessing::imgpath;
std::string imageProcessing::imgOutpath;
Mat imageProcessing::inputImage;
Mat imageProcessing::outputImage;
QPoint imageProcessing::point_0;
QPoint imageProcessing::point_1;

int main(int argc, char *argv[])
{
    //Mat baImage;
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(600,400);//固定窗口大小
    w.show();
    return a.exec();
}
