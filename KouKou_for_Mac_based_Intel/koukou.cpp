#include "koukou.h"
#include "ui_koukou.h"

using namespace cv;

QString filePath,fileName,outPath,Tem;
std::string filepath,outpath,tem;
static int x_1,y_1,x_2,y_2,x_0,y_0;
Mat input,showImage;
Mat backgroud = Mat(Size(1920,1080),CV_8UC3,Scalar(167,167,167));
Scalar backgroudColor=Scalar(167,167,167);

Mat handled(Mat input,Mat bG,int x_1,int y_1,int x_2,int y_2){

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

{
   // setFixedSize(790,590);
    initUI();


#if 0

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
            Mat output = handled(input,backgroud,x_1,y_1,x_2,y_2);
            qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

            showImage=output.clone();
            cv::resize(showImage,showImage,Size(560,560));
            qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
           // ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

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
#endif

}

KouKou::~KouKou()
{
    //delete ui;
}

void KouKou::initUI(){
    QPushButton *testBtn = new QPushButton;

    QSize toolBtnSize(90,60);
    toolBtnBox_01 = new toolBtnBox(0,":/Background_res/0.jpg",":/Background_res/1.jpg",toolBtnSize);
    toolBtnBox_02 = new toolBtnBox(0,":/Background_res/2.jpg",":/Background_res/3.jpg",toolBtnSize);

    QBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(testBtn);
    rightLayout->addWidget(toolBtnBox_01,1);
    rightLayout->addWidget(toolBtnBox_02,1);
    rightLayout->setContentsMargins(0,0,0,0);

   setLayout(rightLayout);
}

//toolBtn_box构造函数
toolBtnBox::toolBtnBox(QWidget *parent,const QString &picPath1, const QString &picPath2,const QSize iconSize){

    btn1 = new QToolButton;
    btn2 = new QToolButton;

    btn1->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn1->setFixedSize(QSize(iconSize));
    btn1->setIcon(QIcon(picPath1));
    btn1->setIconSize(iconSize);

    btn2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn2->setFixedSize(QSize(iconSize));
    btn2->setIcon(QIcon(picPath2));
    btn2->setIconSize(iconSize);

    QBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(btn1,1);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(btn2,1);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);

    connect (btn1,&QToolButton::triggered,[=](){
        QFile file(picPath1);
        if(!file.open(QFile::ReadOnly))
            qDebug()<<" 背景图片加载失败";
        else{
            QByteArray tempBA = file.readAll();
            backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
        }

    });

    connect (btn2,&QToolButton::triggered,[=](){
        QFile file(picPath2);
        if(!file.open(QFile::ReadOnly))
            qDebug()<<" 背景图片加载失败";
        else{
            QByteArray tempBA = file.readAll();
            backgroud = imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
        }

    });
}

toolBtnBox::~toolBtnBox(){

}

/*
void toolBtnBox::setToolBtns(const QString &picPath1, const QString &picPath2,const QSize iconSize){

    btn1->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn1->setFixedSize(QSize(iconSize));
    btn1->setIcon(QIcon(picPath1));
    btn1->setIconSize(iconSize);

    btn2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn2->setFixedSize(QSize(iconSize));
    btn2->setIcon(QIcon(picPath2));
    btn2->setIconSize(iconSize);


}
*/

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
    //ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。


}

//拖入事件
void KouKou::dragEnterEvent(QDragEnterEvent*event){
//如果类型是jpg或者png才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("JPG")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("PNG"))
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
     ///ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

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
    Mat output = handled(input,backgroud,x_1,y_1,x_2,y_2);

    qDebug()<<"outImage:"<< cv::imwrite(outpath,output);

    showImage=output.clone();
    cv::resize(showImage,showImage,Size(560,560));
    qDebug()<<"showImage:"<< cv::imwrite(tem,showImage);
    //ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

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
    //cv::line(src_1,Point(x_1,y_1),Point(x_0,y_0),Scalar(250,0,0),3,LINE_4);
    cv::rectangle(src_1,Point(x_1,y_1),Point(x_0,y_0),Scalar(250,0,0),3,LINE_4);

    qDebug()<<"showImage:"<< cv::imwrite(tem,src_1);
    //ui->label->setPixmap(QPixmap(Tem));//在label里显示图片。

   }
}

