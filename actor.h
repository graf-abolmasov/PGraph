#ifndef ACTOR_H
#define ACTOR_H

#include <QtCore>

class Actor
{
public:
    enum Type {normalType, inlineType};
    Actor(QString extName, QString name, Type type);
    QString extName;
    QString name;
    Type type;
};

#endif // ACTOR_H
