#include "mainwindow.h"

#include <QApplication>
#include <QDateTime>

int main(int argc, char *argv[])
{
    qsrand(QDateTime::currentMSecsSinceEpoch());

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
