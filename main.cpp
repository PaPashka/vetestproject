#include "vemainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VeMainWindow w;
    w.show();
    return a.exec();
}
