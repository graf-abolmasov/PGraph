#include <QtGui/QApplication>
#include "qmywindow.h"
#include <QTextCodec>
#include <time.h>
#include "toppropertydialog.h"

int main(int argc, char *argv[])
{  	
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));
    QApplication a(argc, argv);
    TMyWindow w;
    srand(time(NULL));
    w.show();

    return a.exec();
}
