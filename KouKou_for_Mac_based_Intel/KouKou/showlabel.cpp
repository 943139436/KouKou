#include "showlabel.h"
#include<QDebug>
showLabel::showLabel(QWidget *parent) : QWidget(parent)
{

    showLabelSize = QSize(400,400);
    setAcceptDrops(true);

    label = new QLabel;
    label->setFixedSize(showLabelSize);
    QBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);

}

//绘图事件，使用update（）更新。
void showLabel::paintEvent(QPaintEvent*)
{

   imgToShow = QPixmap(imgPath);//用于QT显示
   imgToShow.scaled(showLabelSize.width(),showLabelSize.height());
   imgRotation(imgPath);

   QRect rec(QPoint(0,0),QPoint(showLabelSize.width(),showLabelSize.height()));
   QRect rec1(point_0,point_1);

   QPainter painter(this);

   if(!imgToShow.isNull()){

   painter.drawPixmap(rec,imgToShow);
   painter.drawRect(rec1);
   }
}

//拖入事件
void showLabel::dragEnterEvent(QDragEnterEvent*event){
//如果类型是jpg或者png才能接受拖动。
//这里的compare字符串比较函数，相等的时候返回0，所以要取反
    qDebug()<<"成功拖入文件";
   if(!event->mimeData()->urls()[0].fileName().right(3).compare("jpg")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("png")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("JPG")
           ||!event->mimeData()->urls()[0].fileName().right(3).compare("PNG"))
       event->acceptProposedAction();
    else
       event->ignore();//否则不接受鼠标事件
}

//放下事件,获取文件路径
void showLabel::dropEvent(QDropEvent*event){


    const QMimeData*qm=event->mimeData(); //获取拖入图像路径
     tempImgPath= qm->urls()[0].toLocalFile();
     imgPath= tempImgPath;
     imgpath = imgPath.toStdString();

     inputImage=imread(imgpath);//用于openCV处理

     QFileInfo fi(imgPath);//生成输出图像路径imgOutpath
     imgName = fi.fileName();
     QString  outPath = imgPath.left(imgPath.size()-imgName.length());
     outPath+="KouKou.jpg";
     imgOutpath = outPath.toStdString();

     point_0 = QPoint(0,0);//初始化矩形框的位置
     point_1 = QPoint(0,0);
     update();

}

//鼠标按下
void showLabel::mousePressEvent(QMouseEvent *ev)
{


    //当鼠标左键按下
    if( ev->button() ==  Qt::LeftButton)
    {

        point_0.setX(ev->x());
        point_0.setY(ev->y());

        if(!inputImage.empty()){
        imgPath = tempImgPath;
        update();
        }
    }
}

//鼠标释放
void showLabel::mouseReleaseEvent(QMouseEvent *ev)
{

    if( ev->button() ==  Qt::LeftButton&&!inputImage.empty())
    {
        imgProcess();
    }



}

//鼠标移动的同时画矩形框，终点为鼠标当前位置
void showLabel::mouseMoveEvent(QMouseEvent *ev)
{
    if( ev->buttons() &   Qt::LeftButton )
    {
    point_1.setX(ev->x());
    point_1.setY(ev->y());
    update();//更新绘画事件
   }
}

void showLabel::imgProcess(){
    outputImage = changeBackground(inputImage,bgImage,point_0,point_1);//生成并保存输出图像
    cv::imwrite(imgOutpath,outputImage);

    imgPath =QString::fromStdString(imgOutpath);//更新显示图像
    update();
}

void showLabel::imgRotation(const QString &imgPath){

    QImageReader reader(imgPath);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    QImageIOHandler::Transformations transformation;
//    if (newImage.isNull())
//     {
//       QMessageBox::information(nullptr, QGuiApplication::applicationDisplayName(),
//                                tr("Cannot load %1: %2")
//                              .arg(QDir::toNativeSeparators(imgPath), reader.errorString()));
//              //return;
//  }
//    else {
          transformation = reader.transformation();
          qDebug() << "transformation is " << transformation;
//    }
    if (transformation == QImageIOHandler::TransformationRotate90
                || transformation == QImageIOHandler::TransformationMirrorAndRotate90
                || transformation == QImageIOHandler::TransformationFlipAndRotate90
                || transformation == QImageIOHandler::TransformationRotate270)
    {
           QTransform matrix;
           matrix.rotate(90);
           imgToShow = imgToShow.transformed(matrix);
    }
    else if (transformation == QImageIOHandler::TransformationRotate180)
    {
           QTransform matrix;
           matrix.rotate(180);
           imgToShow = imgToShow.transformed(matrix);
    }

}
