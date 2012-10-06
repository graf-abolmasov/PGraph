#ifndef NATIVEABSTRACTCOMPILER_H
#define NATIVEABSTRACTCOMPILER_H

#include <QtCore>
#include "abstractcompiler.h"

class NativeCompiler: public QObject, AbstractCompiler
{
    Q_OBJECT

private:

    QProcess *buildScript;
    QTime t;
    QString sh;
public:
    virtual bool compile();
    NativeCompiler(QObject *parent);
    ~NativeCompiler();

private slots:
    void finished(int exitCode, QProcess::ExitStatus);
    void readyRead();
};

#endif // NATIVEABSTRACTCOMPILER_H
