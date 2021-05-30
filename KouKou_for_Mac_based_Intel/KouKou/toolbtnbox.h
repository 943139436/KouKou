#ifndef TOOLBTNBOX_H
#define TOOLBTNBOX_H
#include<QWidget>
#include<QToolButton>
#include<QBoxLayout>
#include<QFile>
#include<QDebug>
#include<QString>
#include<QAction>
#include<QSize>
#include<showlabel.h>

#include<imageprocessing.h>

class toolBtnBox:public QWidget,public imageProcessing{

    Q_OBJECT

public:
    toolBtnBox(QWidget *parent = 0,const QString &picPath1=0,
               const QString &picPath2=0,const QSize iconSize = QSize(0,0));//toolBtn_box构造函数:构造一行两个的toolBtn
    ~toolBtnBox();

    void toolBtnBoxConnect(showLabel *a);

private:
    QToolButton *btn1;
    QToolButton *btn2;
    QString picpath1;
    QString picpath2;

};


#endif // TOOLBTNBOX_H
