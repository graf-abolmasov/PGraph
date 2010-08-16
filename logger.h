#ifndef LOGGER_H
#define LOGGER_H
#include <QtCore>
#include <QtGui>

class Logger
{
public:
    Logger(QString fileName);
    ~Logger();
    void writeLog(QString message);
private:
    QFile *logFile;
};

extern Logger *globalLogger;

#endif // LOGGER_H
