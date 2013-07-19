#include "externalcompiler.h"
#include "../../src/common/qgraphsettings.h"
#include "../../src/common/globalvariables.h"

ExternalCompiler::ExternalCompiler(QObject *parent)
    : QObject(parent)
{
    buildScript = NULL;
    QSettings c(globalSettings->getConfigPath(), QSettings::IniFormat);
    sh = c.value("Compiler/ShCmd", "sh").toString();
}

bool ExternalCompiler::compile()
{
    if (buildScript != NULL && buildScript->state() != QProcess::NotRunning)
        return false;
    buildScript = new QProcess();
    buildScript->setProcessChannelMode(QProcess::MergedChannels);
    globalLogger->log(QObject::tr("Компиляция проекта %1.....").arg(globalDBManager->getProjectName()), Logger::Compile);
    t.start();
    buildScript->setWorkingDirectory(globalSettings->getOutputDirectory());
    buildScript->start(sh + " runext.bat", QProcess::ReadOnly);
    connect(buildScript, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(finished(int,QProcess::ExitStatus)));
    connect(buildScript, SIGNAL(readyRead()), this, SLOT(readyRead()));
    return true;
}

void ExternalCompiler::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus != QProcess::CrashExit)
        globalLogger->log(QObject::tr("Компиляция завершена без ошибок за %1 с.").arg(QString::number(qRound(t.elapsed()/1000.0))), Logger::Compile);
    else
        globalLogger->log(QObject::tr("Компиляция провалилась.").arg(QString::number(qRound(t.elapsed()/1000.0))), Logger::Compile);
    buildScript->close();
    delete buildScript;
    buildScript = NULL;
}

void ExternalCompiler::readyRead()
{
    QTextStream ts(buildScript);
    globalLogger->log(ts.readAll(), Logger::Compile);
}

ExternalCompiler::~ExternalCompiler()
{
    if (buildScript == NULL) return;
    if (buildScript->state() != QProcess::NotRunning)
        buildScript->kill();
    delete buildScript;
}
