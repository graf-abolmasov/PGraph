#include "compiler.h"
#include "../../src/common/globalvariables.h"

Compiler::Compiler()
{
    nativeCompiler = new NativeCompiler(NULL);
    sourceCompiler = new SourceCompiler();
    graphCompiler = NULL;
}

Compiler *Compiler::getCompiler()
{
    return new Compiler();
}

Compiler::~Compiler() {
    if (graphCompiler != NULL)
        delete graphCompiler;
    delete nativeCompiler;
    delete sourceCompiler;
}

QStringList Compiler::compile(const QString &graph)
{
    QStringList errors;
    graphCompiler = new GraphCompiler();

    try {
        const QList<GraphStruct> graphStructs = graphCompiler->compile(globalDBManager->getGraphDB(graph));
        if (graphStructs.size() > 0) {
            errors.append(sourceCompiler->compileCode(graphStructs));
            if (errors.isEmpty())
                errors.append(sourceCompiler->compileData(globalDBManager->getVariableList()));
        }
    } catch (...) {

    }

    delete graphCompiler;
    graphCompiler = NULL;
    return errors;
}
