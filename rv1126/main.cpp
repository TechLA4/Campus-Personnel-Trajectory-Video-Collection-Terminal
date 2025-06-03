#include "mainwindow.h"
#include "config.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QScreen>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();



    return a.exec();
}
