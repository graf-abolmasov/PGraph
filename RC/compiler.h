#ifndef COMPILER_H
#define COMPILER_H

#include <QString>
#include <QFile>
#include <QTextStream>


class ICompiler
{
public:
    virtual bool compile() = 0;

protected:
    QString getTemplate(const QString &fileName) const;
};



#endif // COMPILER_H
