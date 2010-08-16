#include "logger.h"

Logger *globalLogger;

Logger::Logger(QString fileName)
{
    /*logFile = new QFile(fileName);
    logFile->reset();
    logFile->open(QFile::WriteOnly);*/
}

void Logger::writeLog(QString message)
{/*
    QString text;
    text.append(QDateTime::currentDateTime().toUTC().toString() + ": " + message + "\r\n");
    logFile->write(text.toUtf8());*/
}

Logger::~Logger()
{
    /*logFile->close();
    delete logFile;*/
}
