#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
#include "qmywindow.h"

int main(int argc, char *argv[])
{  	
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    TMyWindow w;
    srand(time(NULL));
    w.show();

    return a.exec();
}
