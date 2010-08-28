#include "logger.h"

Logger *globalLogger;

Logger::Logger()
{
    QSettings myLoggerSettings("graph.ini", QSettings::IniFormat);
    logFile = NULL;
    if (myLoggerSettings.value("Logger/WriteLog", false).toBool()) {
        myDebugLevel = DebugLevel(myLoggerSettings.value("Logger/DebugLevel", int(Critical)).toInt());
        logFile = new QFile(myLoggerSettings.value("Logger/FileName", "log.txt").toString());
        logFile->reset();
        logFile->open(QFile::WriteOnly);
    }
}

void Logger::writeLog(QString message, DebugLevel level)
{
    if (logFile != NULL && level <= myDebugLevel) {
        QString text;
        text.append(QDateTime::currentDateTime().toUTC().toString() + ": " + message + "\r\n");
        logFile->write(text.toUtf8());
    }
}

Logger::~Logger()
{
    if (logFile != NULL) {
        logFile->close();
        delete logFile;
    }
}
