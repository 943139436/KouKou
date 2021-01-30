QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /usr/local/Cellar/opencv/4.5.1_1/include/opencv4

LIBS += -L/usr/local/Cellar/opencv/4.5.1_1/lib\
-lopencv_core \
 -lopencv_highgui \
 -lopencv_imgproc \
  -lopencv_imgcodecs

SOURCES += \
    main.cpp \
    koukou.cpp

HEADERS += \
    koukou.h

FORMS += \
    koukou.ui

TRANSLATIONS += \
    KouKou_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
ICON = logo.icns
