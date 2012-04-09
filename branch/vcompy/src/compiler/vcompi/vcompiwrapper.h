#ifndef VCOMPIWRAPPER_H
#define VCOMPIWRAPPER_H

#include <QList>
#include <QString>

class CompTop {
public:
    int Top;
    int FirstDef;
    int LastDef;
    int F;
    int Faz;
    int rankT;
    int back;
    QString Name;
    QString NameProt;
    QString SPName;
    QString SMName;
    QString CodeTr;
};

class DefGrf {
public:
    int F;
    int I;
    int Fl;
    int NTop;
    int NambTop;
    int NambPred;
    int ArcType_;
    QString Name;
    QString CodeTr;
};

namespace vcompi {
class TPOData;
class VcompyWrapper
{
private:
    static void init(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs);
    static void makeResult(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs);
public:
    static int vcompy(QList<CompTop> &tops, QList<DefGrf> &graphs);
};
}

#endif // VCOMPIWRAPPER_H
