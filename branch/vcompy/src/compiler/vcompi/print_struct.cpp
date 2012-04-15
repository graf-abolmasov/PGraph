#include "utype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

PROJECT_BEGIN_NAMESPACE
int print_struct(COMPTOPs *ListTop,DEFGRAFs *LstGraf, int *MaxLT, int *MaxGf) 
{
    int i,NTgraf=*MaxGf,NTtop=*MaxLT;


    printf("ListTop\r\n");
    printf("Name\tTop\tFirstDef\tLastDef\tNameProt\t\F\tCodeTr\tFazr\n");


    for(i=0; i < NTtop; i++)
    {
        printf("%s\t%d\t%d\t%d\t%s\t%d\t%s\t%d\t%d\t%s\r\n",
               (*(*ListTop + i)).Name,
               (*(*ListTop+i)).Top,
               (*(*ListTop+i)).FirstDef,
               (*(*ListTop+i)).LastDef,
               (*(*ListTop+i)).NameProt,
               (*(*ListTop+i)).F,
               (*(*ListTop+i)).CodeTr,
               (*(*ListTop+i)).Faz,
               (*(*ListTop + i)).rankT,
               (*(*ListTop + i)).SPName);
    }

    printf("ListGraf\r\n");
    printf("Name\tNambPred\tNambTop\tArcType_\tF\tI\tFl\tNTop\tCodeTr\r\n");
    for(i=0; i < NTgraf; i++)
    {
        printf("%s\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%s\r\n",
               (*(*LstGraf + i)).Name,
               (*(*LstGraf+i)).NambPred,
               (*(*LstGraf+i)).NambTop,
               (*(*LstGraf+i)).ArcType_,
               (*(*LstGraf+i)).F,
               (*(*LstGraf+i)).I,
               (*(*LstGraf+i)).Fl,
               (*(*LstGraf+i)).NTop,
               (*(*LstGraf + i)).CodeTr);
    }
}

PROJECT_END_NAMESPACE
