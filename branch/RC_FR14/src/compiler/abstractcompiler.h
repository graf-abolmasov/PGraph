#ifndef ABSTRACTCOMPILER_H
#define ABSTRACTCOMPILER_H

#include <QString>
#include <QFile>
#include <QTextStream>


class AbstractCompiler
{
public:
//    virtual bool compile(){return false;}

protected:
    QString getTemplate(const QString &fileName) const;
};



#endif // ABSTRACTCOMPILER_H
