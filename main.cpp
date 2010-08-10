#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
#include "qmywindow.h"
#include "qdrawwindow.h"
#include "databasemanager.h"
#include "commonutils.h"
#include "md5.h"

int main(int argc, char *argv[])
{  	
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    TMyWindow w;
    //w.setWindowTitle(tr("Редактирование сообщения"));
    srand(time(NULL));
    rand();
    globalDBManager = new DataBaseManager();
    w.show();

    return a.exec();
}
