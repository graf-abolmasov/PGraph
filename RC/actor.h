#ifndef ACTOR_H
#define ACTOR_H

#include <QtCore>
#include <QtGui>
#include "variable.h"
#include "basemodule.h"

class Actor
{
public:
    enum Type {normalType, inlineType, graphType};
    Actor(QString name, QString extName, Type type, QString baseModule, QList<Variable*> variableList, QStringList varAMList, QImage icon);
    QString extName;
    QString name;
    Type type;
    QList<Variable*> variableList;
    QStringList varAMList;
    QString baseModule;
    QImage icon;
};

#endif // ACTOR_H
