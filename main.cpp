#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    w.setWindowTitle("实验1-8数码");
    w.setWindowIcon(QIcon(":title.ico"));
    w.show();

    return a.exec();
}
