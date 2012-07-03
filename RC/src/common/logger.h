#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore/QString>
#include <QtCore/QFile>

class Logger : public QObject
{
    Q_OBJECT

public:
    enum LogLevel {Critical, Error, Warning, Compile, Debug, All};
    enum Output {File, Console};

    Logger(LogLevel level, QList<Output> output);
    ~Logger();
    void log(QString message, LogLevel level = All);
    void log(QStringList messages, LogLevel level = All);
    void skipLine();

signals:
    void critical(const QString &msg);
    void error(const QString &msg);
    void warning(const QString &msg);
    void compile(const QString &msg);
    void info(const QString &msg);
    void emptyLine();
    void newMessage(const QString &msg);
private:
    LogLevel myLogLevel;
    QList<Output> myLogTo;
};

#endif // LOGGER_H
