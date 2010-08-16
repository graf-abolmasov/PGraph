#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
#include "qmywindow.h"
#include "qdrawwindow.h"
#include "databasemanager.h"
#include "commonutils.h"
#include "md5.h"
#include "logger.h"

#define QT_DEBUG_COMPONENT

int main(int argc, char *argv[])
{  	
    QApplication::addLibraryPath("./");
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QApplication a(argc, argv);
    TMyWindow w;
    globalLogger = new Logger("log.txt");
    globalDBManager = new DataBaseManager();
    w.show();

    int result = a.exec();
    delete globalDBManager;
    delete globalLogger;
    return result;
}
