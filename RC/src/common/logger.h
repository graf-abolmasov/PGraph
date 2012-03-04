#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore/QString>
#include <QtCore/QFile>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {Critical, Warning, Compile, Debug, All};
    enum Output {File, Console};

    Logger(LogLevel level, QList<Output> output);
    ~Logger();
    void log(QString message, LogLevel level = All);
    void skipLine();

signals:
    void newMessage(const QString &msg);
private:
    LogLevel myLogLevel;
    QList<Output> myLogTo;
};

#endif // LOGGER_H
