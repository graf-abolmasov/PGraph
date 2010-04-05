#ifndef ACTOR_H
#define ACTOR_H

#include <QtCore>

class Actor
{
    enum Type {normalType, inlineType};
public:
    Actor();
    Type type() const
        { return myType; }
    QString name()
        { return myName;}
    QString extName()
        {return myExtName;}
private:
    QString myExtName;
    QString myName;
    Type myType;

};

#endif // ACTOR_H
