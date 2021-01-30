#include "koukou.h"
#include "ui_koukou.h"

using namespace cv;

QString filePath,fileName,outPath,Tem;
std::string filepath,outpath,tem;
static int x_1,y_1,x_2,y_2,x_0,y_0;
Mat input,showImage;
Mat backgroud = Mat(Size(1920,1080),CV_8UC3,Scalar(167,167,167));
Scalar backgroudColor=Scalar(167,167,167);

static Scalar randomColor( RNG& rng )
{
    auto icolor = (unsigned) rng;
    return Scalar( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
}

Mat ConnectedComponentsStats(Mat img,int areaThreshold)
{
    // Use connected components with stats
    Mat labels, stats, centroids;
    auto num_objects= connectedComponentsWithStats(img, labels, stats, centroids,4);
    // Check the number of objects detected
    if(num_objects < 2 ){
        qDebug() << "No objects detected" << endl;
    }else{
        qDebug() << "Number of objects detected: " << num_objects - 1 << endl;
    }
    // Create output image coloring the objects and show area
    Mat output= Mat::zeros(img.rows,img.cols, CV_8UC3);
    RNG rng( 0xFFFFFFFF );
    for(auto i=1; i<num_objects; i++){
        //std::cout << "Object "<< i << " with pos: " << centroids.at<Point2d>(i) << " with area " << stats.at<int>(i, CC_STAT_AREA) << endl;

        if(stats.at<int>(i, CC_STAT_AREA)>=areaThreshold){
        Mat mask= labels==i;
        output.setTo(randomColor(rng), mask);
        }

    }
    //qDebug()<<"output.rows: "<<output.rows<<"  "<<"output.cols: "<<"output.cols<<outputType: "<<output.type()<<endl;
    //imshow("Result", output);
    return output;

}

void  changePixelValueOfImage(Mat input, Mat Roi){
    int rows = input.rows;
    int cols = input.cols;
    //std::cout<<"rowls: "<<rows<<"  "<<"cols: "<<cols<<endl;
    for(int i=0;i<rows;i++){
    for(int j=0;j<(cols*3);j++){
        if(Roi.ptr<uchar>(i)[j]==0)
            input.ptr<uchar>(i)[j]=0;
    }}
}

Mat handled(Mat input,int thresholdValue){
    Mat image =input.clone();
    blur(image, image, Size(3,3));
        medianBlur(image,image,3);

        cvtColor(image, image, COLOR_BGR2GRAY);
        Mat bitimage;
        threshold(image, bitimage, thresholdValue+100, 255, THRESH_BINARY);

        int mode=0;
        Mat Roi;
        if(mode==0)
         Roi=ConnectedComponentsStats(bitimage,10000);
        else {


        }

        Mat output=input.clone();
        changePixelValueOfImage(output,Roi);
        Point seed=Point(0,0);
        floodFill(output, seed, Scalar(180,255,255), nullptr,Scalar(20, 20, 20),Scalar(20,20, 20));
        medianBlur(output, output,3);
        //imshow("output",output);
        return output;

}

Mat handled_2(Mat input,Mat bG,int x_1,int y_1,int x_2,int y_2){

        Mat bg; Mat fg;

        int X1=(int)((x_1*input.cols)/560);
        int Y1=(int)((y_1*input.rows)/560);
        int X2=(int)((x_2*input.cols)/560);
        int Y2=(int)((y_2*input.rows)/560);


        Rect rect = Rect(X1,Y1,X2-X1,Y2-Y1);
        Mat mask,res;
        mask.create( input.size(), CV_8UC1);
        grabCut( input, mask, rect, bg, fg, 1, 0 );

        Mat binMask;
        binMask.create(mask.size(), CV_8UC1 );
        binMask = mask & 1;

        cv:: resize(bG,bG,binMask.size());
        for(auto i = 0;i<binMask.rows;i++)
            for(auto j =0;j<binMask.cols;j++){
                if(binMask.at<uchar>(i,j)!=0){
                bG.at<Vec3b>(i,j)[0]=0;
                bG.at<Vec3b>(i,j)[1]=0;
                bG.at<Vec3b>(i,j)[2]=0;
                }
            }
        input.copyTo( res, binMask );
        res=res+bG;
        medianBlur(res, res,3);

        return res;
}

KouKou::KouKou(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::KouKou)
{
   // setFixedSize(790,590);
    ui->setupUi(this);

    //滑块1改变参数1
    void(QSpinBox:: * spSignal )(int) = &QSpinBox::valueChanged;
        connect(ui->spinBox , spSignal , ui->horizontalScrollBar , &QSlider::setValue);
        connect(ui->horizontalScrollBar, &QSlider::valueChanged,ui->spinBox,&QSpinBox::setValue);
        connect(ui->horizontalScrollBar, &QSlider::valueChanged,[=](){

            parameter_1 = ui->spinBox->value();
            input = cv::imread(filepath);
            Mat output = handled(input,parameter_1);
            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));

            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

//        //滑块2改变参数2
//        connect(ui->spinBox_2 , spSignal , ui->horizontalScrollBar_2 , &QSlider::setValue);
//        connect(ui->horizontalScrollBar_2, &QSlider::valueChanged,ui->spinBox_2,&QSpinBox::setValue);
//        connect(ui->horizontalScrollBar_2, &QSlider::valueChanged,[=](){
//           parameter_2 = ui->spinBox_2->value();
//        });

    //ToolButton选取背景

        //toolButton_2 背景图片
        connect(ui->toolButton_2, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/0.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);
            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

        });

        //toolButton_3 背景图片
        connect(ui->toolButton_3, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/1.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);
            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。


        });

        //toolButton_4 背景图片
        connect(ui->toolButton_4, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/2.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);
            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_5 背景图片
        connect(ui->toolButton_5, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/3.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);
            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_6 背景图片
        connect(ui->toolButton_6, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/4.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_7 背景图片
        connect(ui->toolButton_7, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/5.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_8 背景图片
        connect(ui->toolButton_8, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/6.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_9 背景图片
        connect(ui->toolButton_9, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/7.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_10 背景图片
        connect(ui->toolButton_10, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/8.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });

        //toolButton_11 背景图片
        connect(ui->toolButton_11, &QToolButton::clicked,[=](){

            QFile file(":/Background_res/9.jpg");
            if(!file.open(QFile::ReadOnly))
                qDebug()<< "读取失败" <<endl;
            else
            {
                QByteArray tempBA = file.readAll();
                backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            }

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。
        });
    //自定义按钮，获取color
            connect(ui->pushButton,&QPushButton::clicked,[=](){

            QColor color =  QColorDialog::getColor(QColor(255,0,0));
            backgroud=Mat(Size(1920,1080),CV_8UC3,Scalar(color.blue(),color.green(),color.red()));

            input = cv::imread(filepath);
            Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
            ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。



        });

}

KouKou::~KouKou()
{
    delete ui;
}

//点击菜单获取图片路径
void KouKou::on_actionopen_triggered()
{
    filePath = QFileDialog::getOpenFileName(this, tr("Open File"),"/home",tr("Images (*.png *.xpm *.jpg)"));
    filepath =   filePath.toStdString();

    //生成输出图像路径
    QFileInfo fi(filePath);
    fileName = fi.fileName();
    QString outPath = filePath.left(filePath.size()-fileName.length());
    Tem = outPath;
    outPath+="KouKou.jpg";
    outpath = outPath.toStdString();

    showImage=cv::imread(filepath);
    cv::resize(showImage,showImage,Size(560,560));

    Tem += "KouKou_temp";
    QDir *temp = new QDir;
    bool exist = temp->exists(Tem);
    if(!exist)
    {
        qDebug() <<temp->mkdir(Tem);
    }
    Tem += "/KouKoushow_1.jpg";
    tem = Tem.toStdString();
    qDebug()<<Tem;
    qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
    ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。


}

//拖入事件
void KouKou::dragEnterEvent(QDragEnterEvent*event){
//如果类型是jpg或者png才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("JPG"))
       event->acceptProposedAction();
    else
       event->ignore();//否则不接受鼠标事件
}

//放下事件,获取文件路径
void KouKou::dropEvent(QDropEvent*event){

    //获取拖入图像路径
    const QMimeData*qm=event->mimeData();
     filePath = qm->urls()[0].toLocalFile();
     filepath = filePath.toStdString();

     //生成输出图像路径
     QFileInfo fi(filePath);
     fileName = fi.fileName();
     outPath = filePath.left(filePath.size()-fileName.length());
     Tem = outPath;
     outPath+="KouKou.jpg";
     outpath = outPath.toStdString();

     Mat src=imread(filepath);

     cv::resize(src,src,Size(560,560)); 
     Tem += "KouKou_temp";
     QDir *temp = new QDir;
     bool exist = temp->exists(Tem);
    if(!exist)
     {
         qDebug() <<temp->mkdir(Tem);
     }
     Tem += "/KouKoushow.jpg";
     tem = Tem.toStdString();
     qDebug()<<Tem;
     qDebug()<<"showImage:"<< cv::imwrite(tem,src);
     ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

}

//鼠标按下
void KouKou::mousePressEvent(QMouseEvent *ev)
{

    //当鼠标左键按下
    if( ev->button() ==  Qt::LeftButton)
    {
        x_1=ev->x();
        y_1=ev->y();

    }
}

//鼠标释放
void KouKou::mouseReleaseEvent(QMouseEvent *ev)
{

    if( ev->button() ==  Qt::LeftButton)
    {
        x_2=ev->x();
        y_2=ev->y();
    }

    input = cv::imread(filepath);
    Mat output = handled_2(input,backgroud,x_1,y_1,x_2,y_2);

    qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

    showImage=output.clone();
    cv::resize(showImage,showImage,Size(560,560));
    qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
    ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

}

//鼠标移动,划线，起点为（x_1，y_1），终点为鼠标当前位置
void KouKou::mouseMoveEvent(QMouseEvent *ev)
{
    if( ev->buttons() &   Qt::LeftButton )
    {
    x_0=ev->x();
    y_0=ev->y();
    qDebug()<<x_0;

    Mat src_1=imread(filepath);
    cv::resize(src_1,src_1,Size(560,560));
    cv::line(src_1,Point(x_1,y_1),Point(x_0,y_0),Scalar(167,167,230),8,LINE_4);


    qDebug()<<"showImage:"<< cv::imwrite(tem,src_1);
    ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。




   }
}

