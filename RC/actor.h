#ifndef ACTOR_H
#define ACTOR_H

#include <QtGui/QImage>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QStringList>

class Variable;

class Actor
{
public:
    enum Type {NormalType, InlineType, GraphType};
    Actor(QString name, QString extName, Type type, QString baseModule, QList<Variable*> variableList, QStringList varAMList, QImage icon);
    QString extName;
    QString name;
    Type type;
    QList<Variable* > variableList;
    QStringList varAccModeList;
    QString baseModule;
    QImage icon;
};

#endif // ACTOR_H
