#ifndef SHOWLABEL_H
#define SHOWLABEL_H

#include <QWidget>
#include <QLabel>
#include <QMimeData>
#include<QDragEnterEvent>
#include<QFileInfo>
#include <imageprocessing.h>
#include <QBoxLayout>
#include <QPainter>
#include<QString>
#include<QPixmap>
#include<QImageReader>

class showLabel : public QWidget, public imageProcessing
{
    Q_OBJECT
public:
    explicit showLabel(QWidget *parent = nullptr);

       virtual void mousePressEvent(QMouseEvent *ev);//鼠标按下事件
       virtual void mouseReleaseEvent(QMouseEvent *ev);//鼠标释放事件
       virtual void  mouseMoveEvent(QMouseEvent *ev);//鼠标移动事件

    void imgProcess();//变换背景和更新显示
protected:
    void dragEnterEvent(QDragEnterEvent*event);//鼠标拖动文件进入事件
    void dropEvent(QDropEvent*event);

    void paintEvent(QPaintEvent *event);//绘图事件

    void imgRotation(const QString &imgPath);//处理图片自动旋转信息


private:

    QLabel *label;
    QString imgPath,imgName,tempImgPath;//图像路径和名称（用于QT中图像处理）
    QPixmap imgToShow;//Label中展示的图片
    QSize showLabelSize;

};

#endif // SHOWLABEL_H
