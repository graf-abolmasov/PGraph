#ifndef ACTOR_H
#define ACTOR_H

#include <QtGui/QImage>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>

class Variable;
class BaseModule;

class Actor
{
public:
    enum Type {NormalType, InlineType, GraphType};
    Actor(const Actor &actor);
    Actor(const QString &name, const QString &extName, const Type &type);
    Actor(const QString &name, const QString &extName, const Type &type, const BaseModule *baseModule, const QList<const Variable *> &variableList, const QStringList &varAccModeList, const QImage &icon);
    QString extName;
    QString name;
    Type type;
    QStringList varAccModeList;
    const BaseModule *baseModule;
    QImage icon;
    QList<const Variable *> variableList;
};

#endif // ACTOR_H
