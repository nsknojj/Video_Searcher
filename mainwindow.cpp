#include "mainwindow.h"
#include "example.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    widget = new VWidget();
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/images/sunshine"));
    imageOpen = 0;

    videoPath = new char[255];
    player = NULL;
    playStatus = 0;
    timer = NULL;
    videoWidget = new MyVideoWidget;
    ui->horizontalLayout->addWidget(videoWidget);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    videoWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::open_video(const char *path)
{
    if (player != NULL) delete player;
    strcpy(videoPath, path);
    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QString(path)));//gdi
    player->setVideoOutput(videoWidget);
    ui->horizontalSlider->setRange(0, player->duration());
    connect(player, &QMediaPlayer::positionChanged, [this](qint64 position){
            if(player->duration() != ui->horizontalSlider->maximum())
            {
                ui->horizontalSlider->setMaximum(player->duration());
            }
            ui->horizontalSlider->setValue(player->position());
    });
//    player->play();
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(change_slide()));
    videoWidget->show();

    if (timer == NULL)
        timer = new QTimer(this);
    timer->setInterval(30);
    timer->start();
    connect(timer, SIGNAL(timeout()), videoWidget, SLOT(show()));

//    if (widget->video) delete widget->video;
//    widget->video = new Video(path);
//    ui->horizontalSlider->setRange(0, cvGetCaptureProperty(widget->video->capture, CV_CAP_PROP_FRAME_COUNT));
//    connect(widget->video->timer, SIGNAL(timeout()), widget, SLOT(video_next_frame()));
//    connect(widget->video->timer, SIGNAL(timeout()), this, SLOT(change_slide()));
//    widget->video->change_pos(0);
//    widget->video_next_frame();
//    widget->video->change_pos(0);
//    widget->resize(widget->video->frame->width, widget->video->frame->height);
}

void MainWindow::open_image(const char *path)
{
    int len = strlen(path);
    if (imageOpen) {
        delete[] imagePath;
        cvReleaseImage(&img);
        cvDestroyAllWindows();
        //
    }
    imageOpen = 1;
    imagePath = new char[len + 10];
    strcpy(imagePath, path);
    img = cvLoadImage(imagePath);
    cvNamedWindow("frame", CV_WINDOW_AUTOSIZE);
    cvShowImage("frame", img);
    cvWaitKey(0);
    cvReleaseImage(&img);
    cvDestroyAllWindows();
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
    QString path = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    ".",
                                                    tr("Image(*.png *.gif *.jpg)"));
    if(!path.isEmpty()) {
        open_image(path.toStdString().c_str());
    } else {
//        QMessageBox::warning(this, tr("Path"),
//                             tr("You did not select any file."));
    }
}

void MainWindow::on_actionPlay_Stop_triggered()
{
    if (player == NULL) return;
    if (playStatus == 0) {
            cerr<<player->duration();
        ui->actionPlay_Stop->setIcon(QIcon(":/images/stop"));
        playStatus = 1;
        player->play();
    } else {
        ui->actionPlay_Stop->setIcon(QIcon(":/images/play"));
        playStatus = 0;
        player->pause();
    }
}


void MainWindow::change_slide()
{
    int p = (int)player->position();
    ui->horizontalSlider->setSliderPosition(p);
}


void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
}

void MainWindow::on_horizontalSlider_sliderPressed()
{
//    player->pause();
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    if (playStatus == 1) {
        player->play();
    }
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
//    action
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    player->setPosition(value);
    videoWidget->show();
}

void MainWindow::on_actionSearch_triggered()
{
    double pos = Test(videoPath, imagePath);
    player->setPosition((qint64)(pos * player->duration()));
    videoWidget->show();
}
