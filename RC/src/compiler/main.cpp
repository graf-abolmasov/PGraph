#include <QtGui>
#include <QtCore>
#include <time.h>
#include "../../src/editor/qmywindow.h"
#include "../../src/editor/qdrawwindow.h"
#include "../../src/common/databasemanager.h"
#include "../../src/common/commonutils.h"
#include "../../src/common/md5.h"
#include "../../src/common/logger.h"
#include "../../src/common/globalvariables.h"
#include "../../src/compiler/compiler.h"
#include "../../src/compiler/datacompiler.h"
#include "../../src/compiler/nativecompiler.h"

int main(int argc, char *argv[])
{
    QApplication::addLibraryPath("./");
    QCoreApplication a(argc, argv);
    a.setApplicationVersion("05.02.2012 19.05");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    a.installTranslator(&qtTranslator);

    QList<Logger::Output> logOutput;
    logOutput.append(Logger::Console);
    globalLogger = new Logger(Logger::All, logOutput);

    if (argc > 2) {
        globalDBManager = new DataBaseManager();
        const int projectId = QString::fromAscii(argv[1]).toInt();
        const QString graphName(argv[2]);
        globalDBManager->openProjectDB(projectId);
        const Graph graph = globalDBManager->getGraphDB(graphName);
        globalLogger->log(QObject::tr("Компиляция агрегата %1 из проекта %2").arg(graphName).arg(QString::number(projectId)));

        if (globalDBManager->lastError().type() != QSqlError::NoError) {
            globalLogger->log(QObject::tr("Ошибка подключения к базе данных"), Logger::Critical);
            return -1;
        }

        DataCompiler dataCompiler;
        dataCompiler.compile();

        GraphCompiler graphCompiler(graph);
        graphCompiler.compile();

        delete globalDBManager;
    }
    delete globalLogger;
    return 0;
}
