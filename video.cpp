#include "video.h"

Video::Video(const char *filename)
{
    int len = strlen(filename);
    path = new char[len + 10];
    strcpy(path, filename);
    capture = cvCaptureFromFile(path);
    frame = cvQueryFrame(capture);
    playStatus = 0;
    timer = new QTimer(this);
//    std::cerr<<(int)round(1000.0/cvGetCaptureProperty(capture, CV_CAP_PROP_FPS));
    timer->setInterval((int)round(1000.0/cvGetCaptureProperty(capture, CV_CAP_PROP_FPS)));
    timer->setInterval(60);
    timer->stop();

    qImg = new QImage(QSize(frame->width,frame->height), QImage::Format_RGB888);
    iplImg = cvCreateImageHeader(cvSize(frame->width, frame->height), 8,3);
    iplImg->imageData = (char*)qImg->bits();

}

Video::~Video()
{
    cvReleaseImageHeader(&iplImg);
    cvReleaseCapture(&capture);
    cvReleaseImage(&iplImg);

    if (qImg) delete qImg;
    if (path) delete[] path;
    if (timer) delete timer;
}

void Video::change_pos(int position)
{
    cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, position);
}
