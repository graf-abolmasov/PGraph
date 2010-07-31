#ifndef PREDICATE_H
#define PREDICATE_H

#include <QString>

class Predicate
{
public:
    enum Type {normalType, inlineType};
    Predicate(QString name, QString extName, Type type);
    QString extName;
    QString name;
    Type type;
};

#endif // PREDICATE_H
