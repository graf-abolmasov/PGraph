#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore/QString>
#include <QtCore/QFile>

class Logger
{
public:
    enum LogLevel {Critical, Warning, Compile, Debug, All};
    enum Output {File, Console, Window};

    Logger(LogLevel level, QList<Output> output);
    ~Logger();
    void log(QString message, LogLevel level = All) const;
    void skipLine() const;
private:
    LogLevel myLogLevel;
    QList<Output> myLogTo;
};

#endif // LOGGER_H
