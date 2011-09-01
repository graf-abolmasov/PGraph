#include <QtGui/QListWidget>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#include "logger.h"
#include "globalvariables.h"

Logger *globalLogger;

Logger::Logger(LogLevel level, QList<Output> output)
{
    myLogLevel = level;
    myLogTo = output;
    globalOutput = new QListWidget();
}

Logger::~Logger()
{
    delete globalOutput;
}

void Logger::skipLine() const
{
    if (myLogTo.contains(File)) {
        QFile logFile("log.txt");
        logFile.open(QFile::Append);
        logFile.write("\r\n");
        logFile.close();
    }
    if (myLogTo.contains(Console))
        qDebug() << "";
    if (myLogTo.contains(Window))
        globalOutput->addItem("");
}

void Logger::log(QString message, LogLevel level) const
{
    if (myLogLevel < level)
        return;
    const QString msg = QDateTime::currentDateTime().toUTC().toString() + ": " + message;
    if (myLogTo.contains(File)) {
        QFile logFile("log.txt");
        logFile.open(QFile::Append);
        logFile.write(msg.toUtf8());
        logFile.write("\r\n");
        logFile.close();
    }
    if (myLogTo.contains(Console)) {
        qDebug() << msg;
    }
    if (myLogTo.contains(Window)) {
        globalOutput->addItem(msg);
        globalOutput->scrollToBottom();
    }
}
