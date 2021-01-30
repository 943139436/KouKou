#include "koukou.h"
#include <QApplication>

using namespace cv;

int KouKou::parameter_1 = 100;


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    KouKou w;
    w.show();
    return a.exec();
}

