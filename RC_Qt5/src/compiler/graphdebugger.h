#ifndef GRAPHDEBUGGER_H
#define GRAPHDEBUGGER_H

#include <QtCore>

class GraphDebugger: public QObject
{
    Q_OBJECT

private:
    QProcess *runScript;
    QTime t;
    QString sh;
public:
    bool run();
    GraphDebugger(QObject *parent);
    ~GraphDebugger();

private slots:
    void finished(int exitCode, QProcess::ExitStatus);
    void readyRead();
};

#endif // GRAPHDEBUGGER_H
