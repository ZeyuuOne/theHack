#include "mainwindow.h"
#include <QApplication>
#include "adminregisterdialog.h"
#include "userdatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
