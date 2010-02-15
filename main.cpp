#include <QtGui/QApplication>
#include "qmywindow.h"
#include <QTextCodec>

int main(int argc, char *argv[])
{  	
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QApplication a(argc, argv);
    TMyWindow w;
    w.show();
    return a.exec();
}
