#include<imageprocessing.h>

imageProcessing::imageProcessing(){

}

Mat imageProcessing::changeBackground(Mat inputImg,Mat bgImg,QPoint point_0,QPoint point_1){

        Mat bg; Mat fg;

        int X1=(int)(point_0.x()*inputImg.cols/400);//换算矩形框实际大小为显示的矩形框的 图片大小/560 倍
        int Y1=(int)(point_0.y()*inputImg.rows/400);
        int X2=(int)(point_1.x()*inputImg.cols/400);
        int Y2=(int)(point_1.y()*inputImg.rows/400);

       if(X1==X2||Y1 ==Y2) return inputImg;

        Rect  rect = Rect(min(X1,X2),min(Y1,Y2),abs(X2-X1),abs(Y2-Y1));

        //抠图
        Mat mask,res;
        mask.create( inputImg.size(), CV_8UC1);
        grabCut( inputImg, mask, rect, bg, fg, 1, 0 );
        Mat binMask;
        binMask.create(mask.size(), CV_8UC1 );
        binMask = mask & 1;

        //替换背景
        cv:: resize(bgImg,bgImg,binMask.size());
        for(auto i = 0;i<binMask.rows;i++)
            for(auto j =0;j<binMask.cols;j++){
                if(binMask.at<uchar>(i,j)!=0){
                bgImg.at<Vec3b>(i,j)[0]=0;
                bgImg.at<Vec3b>(i,j)[1]=0;
                bgImg.at<Vec3b>(i,j)[2]=0;
                }
            }
        inputImg.copyTo( res, binMask );
        res=res+bgImg;

        //中值滤波，使图像边缘更平滑
        medianBlur(res, res,3);

        return res;
}

