#ifndef MYVIDEOWIDGET_H
#define MYVIDEOWIDGET_H

#include <QtMultimedia/QMediaPlayer>
#include <QVideoWidget>


class MyVideoWidget : public QVideoWidget
{
public:
    MyVideoWidget();
    void paintEvent(QPaintEvent *event);
};

#endif // MYVIDEOWIDGET_H
