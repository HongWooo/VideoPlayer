#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <qdebug.h>
#include <QIcon>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/qss/main.qss");
    qss.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(qss.readAll());

    a.setStyleSheet(styleSheet);
    qss.close();

    MainWindow w;
    w.showMaximized();
    w.show();

    return a.exec();
}
