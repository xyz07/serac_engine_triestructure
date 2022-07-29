#define _CRT_SECURE_NO_WARNINGS
#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.showInitialLoading();
    w.show();

    return a.exec();
}
