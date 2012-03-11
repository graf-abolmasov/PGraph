#include <QtGui>
#include <QtCore>
#include <time.h>
#include "../../src/editor/qmywindow.h"
#include "../../src/editor/qdrawwindow.h"
#include "../../src/editor/dialogs/projectdialog.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/md5.h"
#include "../../src/common/logger.h"
#include "../../src/common/globalvariables.h"
#include "../../src/compiler/compiler.h"

int main(int argc, char *argv[])
{  	
    QApplication::addLibraryPath("./");
    QApplication a(argc, argv);
    a.setApplicationVersion("11.03.2012 14.53");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    a.installTranslator(&qtTranslator);

    QList<Logger::Output> logOutput;
    logOutput.append(Logger::Console);
    globalLogger = new Logger(Logger::Debug, logOutput);
    globalDBManager = new DataBaseManager();

    if (globalDBManager->lastError().type() != QSqlError::NoError) {
        QMessageBox::critical(NULL, QObject::tr("Ошибка"), QObject::tr("Ошибка подключения к базе данных.\n") + globalDBManager->lastError().databaseText(), QMessageBox::Ok);
        return -1;
    }

    int result = -1;
    ProjectDialog dlg;
    if (dlg.exec()) {
        TMyWindow w;
        w.showMaximized();
        result = a.exec();
    } else QMessageBox::critical(NULL, QObject::tr(ERR_TITLE), QObject::tr("Не выбран проект (aka Предметная область)"));

    delete globalDBManager;
    delete globalLogger;
    return result;
}
