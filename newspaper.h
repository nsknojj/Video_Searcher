#ifndef NEWSPAPER_H
#define NEWSPAPER_H

//!!! Qt5
#include <QObject>

////////// newspaper.h
class Newspaper : public QObject
{
    Q_OBJECT
public:
    Newspaper(const QString & name) :
        m_name(name)
    {
    }

    void send()
    {
        emit newPaper(m_name);
    }

signals:
    void newPaper(const QString &name);

private:
    QString m_name;
};


#endif // NEWSPAPER_H

