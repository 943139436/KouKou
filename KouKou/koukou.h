#ifndef KOUKOU_H
#define KOUKOU_H

#include <QMainWindow>
#include <QColorDialog>
#include <QDebug>
#include <QSlider>
#include <QSpinBox>
#include <QFileDialog>
#include <QString>
#include<QDragEnterEvent>
#include<QMimeData>
#include<QDropEvent>
#include<QUrl>
#include <QLabel>
#include <QPainter>



#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>



QT_BEGIN_NAMESPACE
namespace Ui { class KouKou; }
QT_END_NAMESPACE

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


    static int parameter_1;//参数一
    //int parameter_2;//参数二
    ~KouKou();

protected:
    void dragEnterEvent(QDragEnterEvent*event);//拖动进入事件
    void dropEvent(QDropEvent*event);


private slots:
    void on_actionopen_triggered();

private:


    Ui::KouKou *ui;
};


#endif // KOUKOU_H
