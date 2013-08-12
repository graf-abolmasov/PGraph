#include "graphdebugger.h"

#include "../../src/common/qgraphsettings.h"
#include "../../src/common/globalvariables.h"

GraphDebugger::GraphDebugger(QObject *parent)
    : QObject(parent)
{
    runScript = NULL;
    QSettings c(globalSettings->getConfigPath(), QSettings::IniFormat);
    sh = c.value("Compiler/ShCmd", "sh").toString();
}

bool GraphDebugger::run()
{
    if (runScript != NULL && runScript->state() != QProcess::NotRunning)
        return false;
    runScript = new QProcess();
    runScript->setProcessChannelMode(QProcess::MergedChannels);
    globalLogger->log(QObject::tr("Запуск программы %1.....").arg(globalDBManager->getProjectName()), Logger::Compile);
    t.start();
    runScript->setWorkingDirectory(globalSettings->getOutputDirectory());
    runScript->start(sh + " run.bat", QProcess::ReadOnly);
    connect(runScript, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(runScript, SIGNAL(readyRead()), this, SLOT(readyRead()));
    return true;
}

void GraphDebugger::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus != QProcess::CrashExit)
        globalLogger->log(QObject::tr("Программы завершилась за %1 с.").arg(QString::number(qRound(t.elapsed()/1000.0))), Logger::Compile);
    else
        globalLogger->log(QObject::tr("Не удалось запустить программу.").arg(QString::number(qRound(t.elapsed()/1000.0))), Logger::Compile);
    runScript->close();
    delete runScript;
    runScript = NULL;
}

void GraphDebugger::readyRead()
{
    QTextStream ts(runScript);
    globalLogger->log(ts.readAll(), Logger::Compile);
}

GraphDebugger::~GraphDebugger()
{
    if (runScript == NULL) return;
    if (runScript->state() != QProcess::NotRunning)
        runScript->kill();
    delete runScript;
}
