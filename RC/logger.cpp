#include <QtCore/QSettings>
#include <QtCore/QDebug>
#include <QtCore/QDateTime>

#include "logger.h"

Logger *globalLogger;

Logger::Logger()
{
    QSettings myLoggerSettings("graph.ini", QSettings::IniFormat);
    logFile = NULL;
    toConsole = false;
    if (myLoggerSettings.value("Logger/WriteLog", true).toBool()) {
        myDebugLevel = DebugLevel(myLoggerSettings.value("Logger/DebugLevel", int(All)).toInt());
        QString fileName = myLoggerSettings.value("Logger/FileName", "console").toString();
        if (fileName == "console")
            toConsole = true;
        else {
            toConsole = false;
            logFile = new QFile(fileName);
            logFile->open(QFile::WriteOnly);
            logFile->reset();
        }
    }
}

void Logger::writeLog(QString message, DebugLevel level)
{
    if (logFile != NULL && level <= myDebugLevel) {
        QString text;
        text.append(QDateTime::currentDateTime().toUTC().toString() + ": " + message + "\r\n");
        logFile->write(text.toUtf8());
    } else if (toConsole && level <= myDebugLevel)
        qDebug() << (QDateTime::currentDateTime().toUTC().toString() + ": " + message + "\r\n");
}

Logger::~Logger()
{
    if (logFile != NULL) {
        logFile->close();
        delete logFile;
    }
}
