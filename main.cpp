#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <QPushButton>
//#include "example.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow win;
    win.show();

    return a.exec();
}
