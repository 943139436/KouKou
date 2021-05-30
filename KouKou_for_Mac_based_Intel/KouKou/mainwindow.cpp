#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
   initUI();
   btnConnect();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initUI(){

    QSize toolBtnSzie(91,61);
    toolBtnBox_01 =new toolBtnBox(0,":/Background_res/0.jpg",":/Background_res/1.jpg",toolBtnSzie);
    toolBtnBox_02 = new toolBtnBox(0,":/Background_res/2.jpg",":/Background_res/3.jpg",toolBtnSzie);
    toolBtnBox_03 = new toolBtnBox(0,":/Background_res/4.jpg",":/Background_res/5.jpg",toolBtnSzie);
    toolBtnBox_04 = new toolBtnBox(0,":/Background_res/6.jpg",":/Background_res/7.jpg",toolBtnSzie);
    toolBtnBox_05 = new toolBtnBox(0,":/Background_res/8.jpg",":/Background_res/9.jpg",toolBtnSzie);

    title = new QLabel("选择背景:");
    title->setAlignment(Qt::AlignLeft);
    title->setStyleSheet(QString("QLabel{\
    font-size:14px;color:rgb(200,200,200)}"));

    pushBtn = new QPushButton("自定义纯色背景");


    QBoxLayout *rightLayout = new QVBoxLayout;//界面右边的Layout
    rightLayout->addWidget(title);
    rightLayout->addWidget(toolBtnBox_01);
    rightLayout->addWidget(toolBtnBox_02);
    rightLayout->addWidget(toolBtnBox_03);
    rightLayout->addWidget(toolBtnBox_04);
    rightLayout->addWidget(toolBtnBox_05);
    rightLayout->addWidget(pushBtn);
    rightLayout->setContentsMargins(0,0,0,0);

    showLabel_01 = new showLabel;

    QBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(showLabel_01);
    mainLayout->addLayout(rightLayout);
    mainLayout->setContentsMargins(0,0,0,0);

    QFrame *frame = new QFrame;
    frame->setLayout(mainLayout);
    setCentralWidget(frame);


}
    void MainWindow::btnConnect(){
        toolBtnBox_01->toolBtnBoxConnect(showLabel_01);
        toolBtnBox_02->toolBtnBoxConnect(showLabel_01);
        toolBtnBox_03->toolBtnBoxConnect(showLabel_01);
        toolBtnBox_04->toolBtnBoxConnect(showLabel_01);
        toolBtnBox_05->toolBtnBoxConnect(showLabel_01);

        connect(pushBtn,&QPushButton::clicked,[=](){
           QColor color = QColorDialog::getColor();
           bgImage=  Mat(cv::Size(1920,1080),CV_8UC3,cv::Scalar(color.blue(),color.green(),color.red()));
           showLabel_01->imgProcess();
        });
    }
