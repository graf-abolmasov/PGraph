#include "utype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <QDebug>

PROJECT_BEGIN_NAMESPACE
int print_struct(COMPTOPs *ListTop,DEFGRAFs *LstGraf, int *MaxLT, int *MaxGf) 
{
    int i,NTgraf=*MaxGf,NTtop=*MaxLT;

    char buff[2048];


    int offset = sprintf(buff, "ListTop=\r\n");


    for(i=0; i < NTtop; i++)
    {
        offset += sprintf(buff + offset, "%s, %d, %d, %d, %s, %d, %s, %d\r\n",
               (*(*ListTop+i)).Name,
               (*(*ListTop+i)).Top,
               (*(*ListTop+i)).FirstDef,
               (*(*ListTop+i)).LastDef,
               (*(*ListTop+i)).NameProt,
               (*(*ListTop+i)).F,
               (*(*ListTop+i)).CodeTr,
               (*(*ListTop+i)).Faz);
    }
    offset += sprintf(buff+offset, "ListTop=\r\n");
    for(i=0; i < NTgraf; i++)
    {
        offset += sprintf(buff + offset, "%s, %d, %d, %d, %d, %d, %d, %d, %s\r\n",
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
    qDebug() << QString::fromAscii(buff, offset);

}

PROJECT_END_NAMESPACE
