#include "logger.h"
#include "qdebug.h"

Logger *globalLogger;

Logger::Logger()
{
    QSettings myLoggerSettings("graph.ini", QSettings::IniFormat);
    logFile = NULL;
    toConsole = false;
    if (myLoggerSettings.value("Logger/WriteLog", false).toBool()) {
        myDebugLevel = DebugLevel(myLoggerSettings.value("Logger/DebugLevel", int(Critical)).toInt());
        QString fileName = myLoggerSettings.value("Logger/FileName", "log.txt").toString();
        if (fileName == "console")
            toConsole = true;
        else {
            toConsole = false;
            logFile = new QFile(fileName);
            logFile->reset();
            logFile->open(QFile::WriteOnly);
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
