#include "src/mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <lxi.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();


    return app.exec();
}
