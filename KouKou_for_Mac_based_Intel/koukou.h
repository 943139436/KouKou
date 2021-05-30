#ifndef KOUKOU_H
#define KOUKOU_H

#include <QMainWindow>
#include <QColorDialog>
#include <QDebug>
#include <QSlider>
#include <QSpinBox>
#include <QFileDialog>
#include <QString>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QUrl>
#include <QLabel>
#include <QPainter>
#include<QToolButton>



#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>



//QT_BEGIN_NAMESPACE
//namespace Ui { class KouKou; }
//QT_END_NAMESPACE

class toolBtnBox:public QWidget{

    Q_OBJECT

public:
    toolBtnBox(QWidget *parent = 0,const QString &picPath1=0, const QString &picPath2=0,const QSize iconSize = QSize(0,0));
    ~toolBtnBox();

   // void setToolBtns(const QString &picPath1,const QString &picPath2,const QSize iconSize);

private:
    QToolButton *btn1;
    QToolButton *btn2;
};


class KouKou : public QMainWindow
{
    Q_OBJECT

public:
    KouKou(QWidget *parent = nullptr);

    //鼠标按下
    virtual void mousePressEvent(QMouseEvent *ev);

    //鼠标释放
    virtual void mouseReleaseEvent(QMouseEvent *ev);

    //鼠标移动
    virtual void  mouseMoveEvent(QMouseEvent *ev);

    //界面初始化
    void initUI();

    ~KouKou();

protected:
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);


private slots:
    void on_actionopen_triggered();

private:
    toolBtnBox * toolBtnBox_01;
    toolBtnBox * toolBtnBox_02;
    toolBtnBox * toolBtnBox_03;
    toolBtnBox * toolBtnBox_04;
    toolBtnBox * toolBtnBox_05;

};


#endif // KOUKOU_H
