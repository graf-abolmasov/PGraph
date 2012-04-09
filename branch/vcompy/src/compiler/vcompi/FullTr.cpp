/*Поиск необработанного участка*/
#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *Cod,char *WR,char *Tip,int *Nurov,int *Kurov);
int FullTr(COMPTOPs *ListTop, DEFGRAFs *LstGraf, int *MaxLT, Char200 *CodT, int *FlGr, int *root, int *Faza)
{ 

    int I,R,FistD,LastD,Faz;
    Char200 C;
    char WR='R',TTip;

    for(I=0;I<(*MaxLT);I++)
    {
	if((*(*ListTop + I)).F==0)
        {
            FistD=(*(*ListTop+I)).FirstDef;
            LastD=(*(*ListTop+I)).LastDef;
            strncpy((char*)C,(char*)(*(*ListTop+I)).CodeTr,199);
            Faz=(*(*ListTop+I)).Faz;
            BcodeTr(&C,&WR,&TTip,&R,&R);
            for(FistD;FistD<=LastD;FistD++)
            {
                if(((*(*LstGraf+FistD)).F==0) && ((TTip=='H')||(TTip=='V')))
                {
                    strncpy((char *)CodT,(char *)(*(*LstGraf + FistD-1)).CodeTr,199);
                    (*root)=I;
                    (*Faza)=Faz;
                    *FlGr=1;
                    return 1;
                }
            }
        }
    }
    *FlGr=0;
    return 1;
}
PROJECT_END_NAMESPACE
