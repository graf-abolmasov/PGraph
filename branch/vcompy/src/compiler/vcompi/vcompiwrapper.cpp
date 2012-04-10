#include "tpodata.h"
#include "vcompiwrapper.h"
#include "memman.h"
#include "unistd.h"
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

PROJECT_BEGIN_NAMESPACE
int GB4995CC8(TPOData *D);
int VcompyWrapper::vcompy(QList<CompTop> &tops, QList<DefGrf> &graphs, int maxLT, int maxGf)
{
    TPOData D(1);

    init(&D, tops, graphs, maxLT, maxGf);

    GB4995CC8(&D);

    makeResult(&D, tops, graphs);
}

void VcompyWrapper::init(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs, int maxLT, int maxGf)
{
    COMPTOPs *ListTop = D->GPcomp;
    DEFGRAFs *LstGraf = D->GPgraf;

    (*ListTop) = (COMPTOP *)calloc(maxLT,sizeof(COMPTOP));
    (*LstGraf) = (DEFGRF *)calloc(maxGf,sizeof(DEFGRF));

    int i = 0;
    foreach (CompTop ct, tops) {
        COMPTOP *it = (*ListTop+i);
        it->Top      = -1;
        it->FirstDef = ct.FirstDef;
        it->LastDef  = ct.LastDef ;
        it->F        = ct.F;
        it->Faz      = ct.Faz;

        strncpy(it->NameProt,ct.NameProt.toStdString().c_str(),8);
        strncpy(it->Name,ct.Name.toStdString().c_str(),8);
        strncpy(it->CodeTr,ct.CodeTr.toStdString().c_str(),3);
        i++;
    }

    i = 0;
    foreach (DefGrf dg, graphs) {
        (*(*LstGraf+i)).I        = -1;
        (*(*LstGraf+i)).Fl       = 0;
        (*(*LstGraf+i)).NTop     = -1;
        (*(*LstGraf+i)).F        = dg.F;
        (*(*LstGraf+i)).NambPred = dg.NambPred;
        (*(*LstGraf+i)).NambTop  = dg.NambTop;
        (*(*LstGraf+i)).ArcType_ = dg.ArcType_;

        strncpy((*(*LstGraf + i)).Name,dg.Name.toStdString().c_str(),8);
        strncpy((*(*LstGraf + i)).CodeTr,dg.CodeTr.toStdString().c_str(),8);
        i++;
    }

    (*(D->MaxLT)) = maxLT;
    (*(D->MaxGf)) = maxGf;
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

        newCompTop.Name = QString::fromAscii((*(*ListTop+i)).Name,8);
        newCompTop.CodeTr = QString::fromAscii((*(*ListTop+i)).CodeTr,8);
        newCompTop.SMName = QString::fromAscii((*(*ListTop+i)).SMName,8);
        newCompTop.SPName = QString::fromAscii((*(*ListTop+i)).SPName,8);
        newCompTop.NameProt = QString::fromAscii((*(*ListTop+i)).NameProt,8);

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

        newDefGrf.Name = QString::fromAscii((*(*LstGraf + i)).Name,8);
        newDefGrf.CodeTr = QString::fromAscii((*(*LstGraf + i)).CodeTr,8);

        graphs.append(newDefGrf);
    }

    free((*ListTop));
    free((*LstGraf));

}

PROJECT_END_NAMESPACE
