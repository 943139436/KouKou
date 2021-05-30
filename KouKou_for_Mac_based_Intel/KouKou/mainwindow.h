#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<toolbtnbox.h>
#include<showlabel.h>
#include<QLabel>
#include<QColorDialog>
#include<QPushButton>
#include <QFrame>

class MainWindow : public QMainWindow,public imageProcessing
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initUI();//界面初始化
    void btnConnect();//按钮信号槽联接

private:
    toolBtnBox *toolBtnBox_01;//第一行背景选择按钮
    toolBtnBox *toolBtnBox_02;//第二行背景选择按钮
    toolBtnBox *toolBtnBox_03;//第三行背景选择按钮
    toolBtnBox *toolBtnBox_04;//第四行背景选择按钮
    toolBtnBox *toolBtnBox_05;//第五行背景选择按钮

    QLabel *title;//“选择背景：”
    QPushButton *pushBtn;//“自定义纯色背景”按钮

   showLabel *showLabel_01;//图片展示Label



};
#endif // MAINWINDOW_H
