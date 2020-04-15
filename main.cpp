#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(false);
    MainWindow w;
    //w.show();

    return a.exec();
}
