#ifndef VWIDGET_H
#define VWIDGET_H

#include <QWidget>
#include <QPainter>
#include "video.h"

class VWidget : public QWidget
{
    Q_OBJECT
protected:
    void paintEvent(QPaintEvent *e);

public:
    explicit VWidget(QWidget *parent = 0);
    Video *video;

signals:

public slots:
    void video_next_frame();
};

#endif // VWIDGET_H
