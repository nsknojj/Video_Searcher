#ifndef VIDEO_H
#define VIDEO_H

#include <cv.h>
#include <highgui.h>
#include <QTimer>
#include <QObject>
#include <QImage>

class Video : public QObject
{
    Q_OBJECT
public:
    Video(const char *filename);
    ~Video();
    char *path;
    CvCapture *capture;
    IplImage *frame, *iplImg;
    QTimer *timer;
    QImage *qImg;
    bool playStatus;

    void change_pos(int position);

signals:
    void pos_changed();

};

#endif // VIDEO_H
