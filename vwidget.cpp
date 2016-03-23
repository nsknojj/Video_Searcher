#include "vwidget.h"

VWidget::VWidget(QWidget *parent) : QWidget(parent)
{
    video = NULL;
}

void VWidget::paintEvent(QPaintEvent *e)
{
    if (video) {
        QPainter painter(this);
        painter.drawImage(QPoint(0,0), *(video->qImg));
    }
}

void VWidget::video_next_frame()
{
    video->frame = cvQueryFrame(video->capture);
    if (video->frame)
    {
        if (video->frame->origin == IPL_ORIGIN_TL)
        {
            cvCopy(video->frame, video->iplImg, 0);
        }
        else
        {
            cvFlip(video->frame, video->iplImg, 0);
        }
        cvCvtColor(video->iplImg, video->iplImg, CV_BGR2RGB);
        this->update();
    }
}
