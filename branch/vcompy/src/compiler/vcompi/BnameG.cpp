/*Формирование имени графа и (*Nobj)++ номера текущего и будущего графа*/
#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int BnameG(NAMAGR *NameGr,int *Nobj)
{
    NAMAGR C;
    char Np[6];
    strncpy((char*)C,"Wg",3);
    sprintf(Np,"%d",(*Nobj));
    strcat(C,Np);
    strncpy((char*)(*NameGr),(char*)C,NAMEPR_SIZE);
    (*Nobj)++;
    return 1;
}
PROJECT_END_NAMESPACE
