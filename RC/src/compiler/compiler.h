#ifndef COMPILER_H
#define COMPILER_H

#include <QtCore>
#include "../../src/compiler/nativecompiler.h"
#include "../../src/compiler/abstractcompiler.h"
#include "../../src/compiler/sourcecompiler.h"
#include "../../src/compiler/graphcompiler.h"


class Compiler
{
private:
    SourceCompiler *sourceCompiler;
    NativeCompiler *nativeCompiler;
    GraphCompiler *graphCompiler;
    Compiler();

public:
    ~Compiler();
    static Compiler *getCompiler();
    QStringList compile(const QString &graph);

};

#endif // COMPILER_H
