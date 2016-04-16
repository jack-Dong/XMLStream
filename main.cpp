#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.readFile("/Users/dongshizhou/qtProjects/XMLStream/bookindex.xml");
    w.show();

    return a.exec();
}
