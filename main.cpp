#include <QApplication>
#include "LayoutFile/layout.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    layout l;
    l.show();

    return a.exec();
}
