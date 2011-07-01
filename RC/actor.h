#ifndef ACTOR_H
#define ACTOR_H

#include <QtGui/QImage>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "variable.h"


class Actor
{
public:
    enum Type {NormalType, InlineType, GraphType};
    Actor(const Actor &actor);
    Actor(QString name, QString extName, Type type);
    Actor(QString name, QString extName, Type type, QString baseModule, QList<const Variable *> variableList, QStringList varAccModeList, QImage icon);
    QString extName;
    QString name;
    Type type;
    QStringList varAccModeList;
    QString baseModule;
    QImage icon;
    QList<const Variable *> variableList;
};

#endif // ACTOR_H
