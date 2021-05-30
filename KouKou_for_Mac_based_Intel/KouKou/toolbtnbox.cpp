#include "toolbtnbox.h"

toolBtnBox::toolBtnBox(QWidget *parent,const QString &picPath1, const QString &picPath2,const QSize iconSize){
    Q_UNUSED(parent);
    picpath1=picPath1;
     picpath2=picPath2;
    btn1 = new QToolButton(this);
    btn2 = new QToolButton(this);

    btn1->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn1->setFixedSize(QSize(iconSize));
    btn1->setIcon(QIcon(picPath1));
    btn1->setIconSize(iconSize);

    btn2->setToolButtonStyle(Qt::ToolButtonIconOnly);
    btn2->setFixedSize(QSize(iconSize));
    btn2->setIcon(QIcon(picPath2));
    btn2->setIconSize(iconSize);

    QBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(btn1,1);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(btn2,1);
    mainLayout->setContentsMargins(0,0,0,0);

    setLayout(mainLayout);

}

toolBtnBox::~toolBtnBox(){

}
void toolBtnBox::toolBtnBoxConnect(showLabel *a){

    connect (btn1,&QToolButton::clicked,[=](){
        QFile file(picpath1);
        if(!file.open(QFile::ReadOnly))
            qDebug()<<" 背景图片加载失败";
        else{
            qDebug()<<"加载图片成功";
            QByteArray tempBA = file.readAll();

            bgImage = cv::imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            if(!inputImage.empty()) a->imgProcess();
        }

    });

    connect (btn2,&QToolButton::clicked,[=](){
        QFile file(picpath2);
        if(!file.open(QFile::ReadOnly))
            qDebug()<<" 背景图片加载失败";
        else{
            QByteArray tempBA = file.readAll();

            bgImage = cv::imdecode(std::vector<char>(tempBA.begin(), tempBA.end()), 1);
            if(!inputImage.empty()) a->imgProcess();
        }
    });
}
