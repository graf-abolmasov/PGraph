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
    TPOData D;

    init(&D, tops, graphs, maxLT, maxGf);

    GB4995CC8(&D);

    makeResult(&D, tops, graphs);
}

void VcompyWrapper::init(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs, int maxLT, int maxGf)
{
    COMPTOPs ListTop = (COMPTOP *)calloc(maxLT,sizeof(COMPTOP));
    DEFGRAFs LstGraf = (DEFGRF *)calloc(maxGf,sizeof(DEFGRF));

    int i = 0;
    foreach (CompTop ct, tops) {
        COMPTOP *it  = ListTop+i;
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
        DEFGRF *it = LstGraf+i;
        it->I        = -1;
        it->Fl       = 0;
        it->NTop     = -1;
        it->F        = dg.F;
        it->NambPred = dg.NambPred;
        it->NambTop  = dg.NambTop;
        it->ArcType_ = dg.ArcType_;

        strncpy(it->Name,dg.Name.toStdString().c_str(),8);
        strncpy(it->CodeTr,dg.CodeTr.toStdString().c_str(),8);
        i++;
    }

    (*(D->MaxLT)) = tops.size();
    (*(D->NumTop)) = tops.size();
    (*(D->MaxGf)) = graphs.size();

    (*(D->GPcomp)) = ListTop;
    (*(D->GPgraf)) = LstGraf;
}

void VcompyWrapper::makeResult(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs)
{
    tops.clear();
    graphs.clear();

    int NTtop  = (*(D->MaxLT));
    int NTgraf = (*(D->MaxGf));

    COMPTOPs ListTop = (*(D->GPcomp));
    DEFGRAFs LstGraf = (*(D->GPgraf));

    for(int i=0; i < NTtop; i++) {
        CompTop newCompTop;
        COMPTOP *it = ListTop + i;
        newCompTop.back = it->back;
        newCompTop.F = it->F;
        newCompTop.Faz = it->Faz;
        newCompTop.FirstDef = it->FirstDef;
        newCompTop.LastDef = it->LastDef;
        newCompTop.rankT = it->rankT;
        newCompTop.Top = it->Top;

        newCompTop.Name = QString::fromAscii(it->Name, 8);
        newCompTop.CodeTr = QString::fromAscii(it->CodeTr, 8);
        newCompTop.SMName = QString::fromAscii(it->SMName, 8);
        newCompTop.SPName = QString::fromAscii(it->SPName, 8);
        newCompTop.NameProt = QString::fromAscii(it->NameProt, 8);

        tops.append(newCompTop);
    }

    for(int i=0; i < NTgraf; i++) {
        DefGrf newDefGrf;
        DEFGRF *it = LstGraf+i;
        newDefGrf.ArcType_ = it->ArcType_;
        newDefGrf.F = it->F;
        newDefGrf.Fl = it->Fl;
        newDefGrf.NambPred = it->NambPred;
        newDefGrf.NambTop = it->NambTop;
        newDefGrf.I = it->I;
        newDefGrf.NTop = it->NTop;

        newDefGrf.Name = QString::fromAscii(it->Name, 8);
        newDefGrf.CodeTr = QString::fromAscii(it->CodeTr, 8);

        graphs.append(newDefGrf);
    }

    free(ListTop);
    free(LstGraf);
}

PROJECT_END_NAMESPACE
