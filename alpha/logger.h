#ifndef LOGGER_H
#define LOGGER_H
#include <QtCore>
#include <QtGui>

class Logger
{
public:
    enum DebugLevel {Critical, All};

    Logger();
    ~Logger();
    void writeLog(QString message, DebugLevel level = All);
private:
    QFile *logFile;
    DebugLevel myDebugLevel;
    bool toConsole;
};

#endif // LOGGER_H
