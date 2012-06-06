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

    // qDeleteAll(tops.begin(), tops.end());
    tops.clear();
    graphs.clear();

    makeResult(&D, tops, graphs);

    return 0;
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

        strncpy(it->NameProt,ct.NameProt.toStdString().c_str(),NAMEPR_SIZE);
        strncpy(it->Name,ct.Name.toStdString().c_str(),NAMEPR_SIZE);
        strncpy(it->CodeTr,ct.CodeTr.toStdString().c_str(),3);
        i++;
    }

    if (maxLT > tops.size())    {
        for( i = tops.size(); i < maxLT; i++ )  {
            COMPTOP *it  = ListTop+i;
            it->Top      = -1;
            it->FirstDef = -77;
            it->LastDef  = -77;
            it->F        = -1;
            it->Faz      = -1;

            it->NameProt[0] = '\0';
            it->Name[0] = '\0';
            strcpy(it->CodeTr, "0.I");

        }
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

        strncpy(it->Name,dg.Name.toStdString().c_str(),NAMEPR_SIZE);
        strncpy(it->CodeTr,dg.CodeTr.toStdString().c_str(),NAMEPR_SIZE);
        i++;
    }

    if ( maxGf > graphs.size() )    {
        for( i = graphs.size(); i < maxGf; i++ )  {
            DEFGRF *it = LstGraf+i;
            it->I        = -1;
            it->Fl       = 0;
            it->NTop     = -1;
            it->F        = -1;
            it->NambPred = -1;
            it->NambTop  = -1;
            it->ArcType_ = -1;

            it->Name[0] = '\0';
            strcpy(it->CodeTr, "0.I");
        }
    }

    ////(*(D->MaxLT)) = tops.size();
    (*(D->MaxLT)) = maxLT;
    (*(D->NumTop)) = tops.size();
    ////(*(D->MaxGf)) = graphs.size();
    (*(D->MaxGf)) = maxGf;

    (*(D->GPcomp)) = ListTop;
    (*(D->GPgraf)) = LstGraf;
}

void VcompyWrapper::makeResult(TPOData *D, QList<CompTop> &tops, QList<DefGrf> &graphs)
{
//    tops.clear();
//    graphs.clear();

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

        // newCompTop.Name = ( it->Name[0]=='\0' ) ? "": QString::fromAscii(it->Name, 8);
        newCompTop.Name = QString::fromAscii(it->Name, NAMEPR_SIZE);
        newCompTop.CodeTr = QString::fromAscii(it->CodeTr, NAMEPR_SIZE);
        newCompTop.SMName = QString::fromAscii(it->SMName, NAMEPR_SIZE);
        newCompTop.SPName = QString::fromAscii(it->SPName, NAMEPR_SIZE);
        newCompTop.NameProt = QString::fromAscii(it->NameProt, NAMEPR_SIZE);

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

        newDefGrf.Name = QString::fromAscii(it->Name, NAMEPR_SIZE);
        newDefGrf.CodeTr = QString::fromAscii(it->CodeTr, NAMEPR_SIZE);

        graphs.append(newDefGrf);
    }

//     free(ListTop);
//     free(LstGraf);
}

PROJECT_END_NAMESPACE
