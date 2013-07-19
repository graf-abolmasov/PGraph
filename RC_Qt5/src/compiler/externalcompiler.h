#ifndef EXTERNALCOMPILER_H
#define EXTERNALCOMPILER_H

#include <QtCore>
#include "abstractcompiler.h"

class ExternalCompiler: public QObject, AbstractCompiler
{
    Q_OBJECT

private:

    QProcess *buildScript;
    QTime t;
    QString sh;
public:
    virtual bool compile();
    ExternalCompiler(QObject *parent);
    ~ExternalCompiler();

private slots:
    void finished(int exitCode, QProcess::ExitStatus);
    void readyRead();
};

#endif // EXTERNALCOMPILER_H
