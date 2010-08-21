#include <QtGui/QApplication>
#include <QTextCodec>
#include <time.h>
#include "qmywindow.h"
#include "qdrawwindow.h"
#include "databasemanager.h"
#include "commonutils.h"
#include "md5.h"
#include "logger.h"

int main(int argc, char *argv[])
{  	
    QApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    a.setApplicationVersion("21.08.2010 18.27");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    a.installTranslator(&qtTranslator);

    //globalLogger = new Logger("log.txt");
    globalDBManager = new DataBaseManager();

    TMyWindow w;
    w.show();

    int result = a.exec();
    delete globalDBManager;
    //delete globalLogger;
    return result;
}
