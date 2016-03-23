#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    widget = new VWidget();
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(widget);
    this->setWindowIcon(QIcon(":/images/sunshine"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_video(const char *path)
{
    if (widget->video) delete widget->video;
    widget->video = new Video(path);
    ui->horizontalSlider->setRange(0, cvGetCaptureProperty(widget->video->capture, CV_CAP_PROP_FRAME_COUNT));
    connect(widget->video->timer, SIGNAL(timeout()), widget, SLOT(video_next_frame()));
    connect(widget->video->timer, SIGNAL(timeout()), this, SLOT(change_slide()));
    widget->video->change_pos(0);
    widget->video_next_frame();
    widget->video->change_pos(0);
    widget->resize(widget->video->frame->width, widget->video->frame->height);
}


void MainWindow::on_actionOpenFile_triggered()
{
    QString path = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    ".",
                                                    tr("Video(*.avi *.mp4 *.rmvb)"));
    if(!path.isEmpty()) {
        open_video(path.toStdString().c_str());
    } else {
//        QMessageBox::warning(this, tr("Path"),
//                             tr("You did not select any file."));
    }
}

void MainWindow::on_actionOpenFrame_triggered()
{

}

void MainWindow::on_actionPlay_Stop_triggered()
{
    if (widget->video == NULL) return;
    if (widget->video->playStatus == 0) {
        ui->actionPlay_Stop->setIcon(QIcon(":/images/stop"));
        widget->video->playStatus = 1;
        widget->video->timer->start();
    } else {
        ui->actionPlay_Stop->setIcon(QIcon(":/images/play"));
        widget->video->playStatus = 0;
        widget->video->timer->stop();
    }
}


void MainWindow::change_slide()
{
    int p = cvGetCaptureProperty(widget->video->capture, CV_CAP_PROP_POS_FRAMES);
    ui->horizontalSlider->setSliderPosition(p);
}


void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    widget->video->timer->stop();
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    if (widget->video->playStatus == 1) {
        widget->video->timer->start();
    }
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
//    action
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    widget->video->change_pos(value);
    widget->video_next_frame();
}
