#ifndef ACTOR_H
#define ACTOR_H

#include <QtGui/QPixmap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "errors.h"

class Variable;
class BaseModule;

class Actor
{
public:
    enum Type {NormalType, InlineType, GraphType};
    Actor(const Actor &actor);
    Actor(const QString &name, const QString &extName, const Type &type);
    Actor(const QString &name, const QString &extName, const Type &type, const BaseModule *baseModule, const QList<const Variable *> &variableList, const QStringList &varAccModeList, const QPixmap &icon);
    QString extName;
    QString name;
    Type type;
    QStringList varAccModeList;
    const BaseModule *baseModule;
    QList<const Variable *> variableList;
    QStringList validate() const;
    void build() const;
    QPixmap icon;
};

#endif // ACTOR_H
