#ifndef PREDICATE_H
#define PREDICATE_H

#include <QtCore/QString>
#include <QtCore/QList>

class Variable;

class Predicate
{
public:
    enum Type {normalType, inlineType};
    Predicate(QString name, QString extName, Type type, QString baseModule, QList<Variable *> variableList);
    QString extName;
    QString name;
    Type type;
    QList<Variable *> variableList;
    QString baseModule;
};

#endif // PREDICATE_H
