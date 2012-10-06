#include "compiler.h"

QString AbstractCompiler::getTemplate(const QString &fileName) const
{
    QString name = fileName;
    Q_ASSERT(QFile::exists(fileName));
    QFile f(name);
    f.open(QIODevice::ReadOnly);
    QTextStream ts(&f);
    ts.setCodec("UTF-8");
    QString result = ts.readAll();
    f.close();
    return result;
}
