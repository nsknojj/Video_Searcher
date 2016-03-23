#-------------------------------------------------
#
# Project created by QtCreator 2016-03-04T11:29:29
#
#-------------------------------------------------

INCLUDEPATH += E:\opencv\opencv\build\include\
INCLUDEPATH += E:\opencv\opencv\build\include\opencv\
INCLUDEPATH += E:\opencv\opencv\build\include\opencv2\

CONFIG(debug,debug|release) {
LIBS += -LE:\opencv\opencv\build\x64\vc12\lib \
    -lopencv_core2410d \
    -lopencv_highgui2410d \
    -lopencv_imgproc2410d \
    -lopencv_features2d2410d \
    -lopencv_calib3d2410d
} else {
LIBS += -LE:\opencv\opencv\build\x64\vc12\lib \
    -lopencv_core2410 \
    -lopencv_highgui2410 \
    -lopencv_imgproc2410 \
    -lopencv_features2d2410 \
    -lopencv_calib3d2410
}

LIBS += -LE:\QTproject\HelloWorld\lib \
	-lEV2641CDVSLib

LIBS += -LE:\QTproject\HelloWorld\dll \
	-lEV2641CDVSLib
	-llibfftw3f-3

QT       += core gui \
	    multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelloWorld
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vwidget.cpp \
    video.cpp

HEADERS  += mainwindow.h \
    vwidget.h \
    video.h \
    EV2641CDVSLib.h \
    example.hpp

RESOURCES += \
    res.qrc

FORMS += \
    mainwindow.ui
