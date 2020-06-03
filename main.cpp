#include "vemainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VEMainWindow w;
    w.show();
    return a.exec();
}
