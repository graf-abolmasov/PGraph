#include "utype.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int AddTop(COMPTOPs *GPcomp,DEFGRAFs *GPgraf,int *I,NAMAGR *WSP,NAMAGR *NameGr,int *FromTop,int *MaxLT)
{
    int n1;
//    NAMAGR SPWSP;

    n1=((*(GPgraf))+(*I))->NambTop;//Вершина преемник FromTop
//    strncpy(SPWSP,WSP,8);
    strncpy(((*(GPcomp))+n1)->SPName,(const char *)WSP,8);
    ((*(GPgraf))+(*I))->NambTop=(*MaxLT);
    strncpy(((*(GPcomp))+(*MaxLT))->Name,(const char *)WSP,8);
    strncpy(((*(GPcomp))+(*MaxLT))->SPName,(const char *)NameGr,8);
    strncpy(((*(GPcomp))+(*MaxLT))->CodeTr,"0.W",8);
    //((*(GPcomp))+n1)->rankT=0;
    ((*(GPcomp))+(*MaxLT))->back=(*FromTop);

    return 1;
}
PROJECT_END_NAMESPACE