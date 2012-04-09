#include "tpodata.h"
#include "vcompiwrapper.h"
#include "memman.h"
#include "unistd.h"
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

PROJECT_BEGIN_NAMESPACE
int GB4995CC8(TPOData *D);
int VcompyWrapper::vcompy(QList<CompTop> &tops, QList<DefGrf> &graphs)
{
    TPOData D(1);

    init(&D, tops, graphs);

    GB4995CC8(&D);

    makeResult(&D, tops, graphs);
}

void VcompyWrapper::init(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs)
{
    int NTtop  = tops.size();
    int NTgraf = graphs.size();

    COMPTOPs *ListTop = D->GPcomp;
    DEFGRAFs *LstGraf = D->GPgraf;

    *ListTop = (COMPTOP *)calloc(NTtop,sizeof(COMPTOP));
    *LstGraf = (DEFGRF *)calloc(NTgraf,sizeof(DEFGRF));

    for(int i=0; i < NTtop; i++) {
        (*(*ListTop+i)).Top      = -1;
        (*(*ListTop+i)).FirstDef = tops[i].FirstDef;
        (*(*ListTop+i)).LastDef  = tops[i].LastDef ;
        (*(*ListTop+i)).F        = tops[i].F;
        (*(*ListTop+i)).Faz      = tops[i].Faz;

        strncpy((*(*ListTop+i)).NameProt,tops[i].NameProt.toStdString().c_str(),8);
        strncpy((*(*ListTop + i)).Name,tops[i].Name.toStdString().c_str(),8);
        strncpy((*(*ListTop+i)).CodeTr,tops[i].CodeTr.toStdString().c_str(),3);
    }

    for(int i=0; i < NTgraf; i++) {
        (*(*LstGraf+i)).I        = -1;
        (*(*LstGraf+i)).Fl       = 0;
        (*(*LstGraf+i)).NTop     = -1;
        (*(*LstGraf+i)).F        = graphs[i].F;
        (*(*LstGraf+i)).NambPred = graphs[i].NambPred;
        (*(*LstGraf+i)).NambTop  = graphs[i].NambTop;
        (*(*LstGraf+i)).ArcType_ = graphs[i].ArcType_;

        strncpy((*(*LstGraf + i)).Name,graphs[i].Name.toStdString().c_str(),8);
        strncpy((*(*LstGraf + i)).CodeTr,graphs[i].CodeTr.toStdString().c_str(),8);
    }

    (*(D->MaxLT)) = NTtop;
    (*(D->NumTop)) = NTtop;
    (*(D->MaxGf)) = NTgraf;
}

void VcompyWrapper::makeResult(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs)
{
    tops.clear();
    graphs.clear();

    int NTtop  = (*(D->MaxLT));
    int NTgraf = (*(D->MaxGf));

    COMPTOPs *ListTop = D->GPcomp;
    DEFGRAFs *LstGraf = D->GPgraf;

    for(int i=0; i < NTtop; i++) {
        CompTop newCompTop;
        newCompTop.back = (*(*ListTop+i)).back;
        newCompTop.F = (*(*ListTop+i)).F;
        newCompTop.Faz = (*(*ListTop+i)).Faz;
        newCompTop.FirstDef = (*(*ListTop+i)).FirstDef;
        newCompTop.LastDef = (*(*ListTop+i)).LastDef;
        newCompTop.rankT = (*(*ListTop+i)).rankT;
        newCompTop.Top = (*(*ListTop+i)).Top;

        newCompTop.Name.fromAscii((*(*ListTop+i)).Name,8);
        newCompTop.CodeTr.fromAscii((*(*ListTop+i)).CodeTr,8);
        newCompTop.SMName.fromAscii((*(*ListTop+i)).SMName,8);
        newCompTop.SPName.fromAscii((*(*ListTop+i)).SPName,8);
        newCompTop.NameProt.fromAscii((*(*ListTop+i)).NameProt,8);

        tops.append(newCompTop);
    }

    for(int i=0; i < NTgraf; i++) {
        DefGrf newDefGrf;
        newDefGrf.ArcType_ = (*(*LstGraf+i)).ArcType_;
        newDefGrf.F = (*(*LstGraf+i)).F;
        newDefGrf.Fl = (*(*LstGraf+i)).Fl;
        newDefGrf.NambPred = (*(*LstGraf+i)).NambPred;
        newDefGrf.NambTop = (*(*LstGraf+i)).NambTop;
        newDefGrf.I = (*(*LstGraf+i)).I;
        newDefGrf.NTop = (*(*LstGraf+i)).NTop;

        newDefGrf.Name.fromAscii((*(*LstGraf + i)).Name,8);
        newDefGrf.CodeTr.fromAscii((*(*LstGraf + i)).CodeTr,8);

        graphs.append(newDefGrf);
    }

    free(*ListTop);
    free(*LstGraf);

}

PROJECT_END_NAMESPACE
