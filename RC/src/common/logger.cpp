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
    myLogTo = output;;
}

Logger::~Logger()
{
}

void Logger::skipLine()
{
    if (myLogTo.contains(File)) {
        QFile logFile("log.txt");
        logFile.open(QFile::Append);
        logFile.write("\r\n");
        logFile.close();
    }
    if (myLogTo.contains(Console))
        qDebug() << "";
    emit newMessage("");
}

void Logger::log(QString message, LogLevel level)
{
    if (myLogLevel < level)
        return;

    const QStringList lines = message.split(QRegExp("[\\n\\r]"), QString::SkipEmptyParts);
    const QString prefix = QDateTime::currentDateTime().toUTC().toString() + ": ";
    QStringList msg;
    foreach (QString line, lines)
        msg.append(prefix + line);
    message = msg.join("\r\n");

    if (myLogTo.contains(File)) {
        QFile logFile("log.txt");
        logFile.open(QFile::Append);
        logFile.write(message.toUtf8());
        logFile.write("\r\n");
        logFile.close();
    }
    if (myLogTo.contains(Console)) {
        qDebug() << message;
    }
    emit newMessage(message);
}
