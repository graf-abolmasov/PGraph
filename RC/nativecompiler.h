#ifndef NATIVECOMPILER_H
#define NATIVECOMPILER_H

#include <QtCore>
#include "compiler.h"

class NativeCompiler: public QObject, ICompiler
{
    Q_OBJECT

private:

    QProcess *buildScript;
    QTime t;
public:
    virtual bool compile();
    NativeCompiler(QObject *parent);
    ~NativeCompiler();

private slots:
    void finished(int exitCode, QProcess::ExitStatus);
    void readyRead();
};

#endif // NATIVECOMPILER_H
