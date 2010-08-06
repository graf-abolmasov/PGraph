#ifndef ACTOR_H
#define ACTOR_H

#include <QtCore>
#include "variable.h"
#include "basemodule.h"

class Actor
{
public:
    enum Type {normalType, inlineType};
    Actor(QString name, QString extName, Type type, QString baseModule, QList<Variable*> variableList);
    QString extName;
    QString name;
    Type type;
    QList<Variable*> variableList;
    QString baseModule;
};

#endif // ACTOR_H
