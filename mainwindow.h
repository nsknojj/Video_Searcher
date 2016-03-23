#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QTimer>
#include <QScrollBar>
#include "ui_mainwindow.h"
#include <cv.h>
#include <highgui.h>
#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>
#include "vwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    VWidget *widget;

    char *imagePath;
    bool imageOpen;
    IplImage *img;

    void open_video(const char *path);
    void open_image(const char *path);

private slots:
    void on_actionOpenFile_triggered();

    void on_actionOpenFrame_triggered();

    void on_actionPlay_Stop_triggered();

    void change_slide();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_actionTriggered(int action);

    void on_horizontalSlider_valueChanged(int value);

    void on_actionSearch_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
