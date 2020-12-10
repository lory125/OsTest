#include "widget.h"
#include "process.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    list<process> readyProcess;//就绪队列
    Widget w;
    w.show();
    return a.exec();
}
