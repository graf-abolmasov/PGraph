#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int BfindE(COMPTOPs *ListTop,DEFGRAFs *LstGraf,Char200 *Cod,int *FromTop,int *MaxLT,int *MaxGf,int *rank,int *Faza)
{
    int I,K,NJ,Ne,FistD,LastD,JT;
    int NumP[500];
    Char200 Cw,C1,C2;
    char *p;

    strncpy((char*)Cw,(char*)Cod,199);
    p = strtok(Cw,"H ");
    strcpy(C1,p);

    if(strcmp(C1,"0.")==0)
    {strcpy((char*)Cw,(char*)C1);
        strncat((char*)Cw,"E",1);
    }
    else
    {
	p = strtok(NULL, "H ");
	strcpy(C2,p);
	strcpy((char*)Cw,(char*)C1);
	strncat((char*)Cw,"V",1);
	strcat((char*)Cw,(char*)C2);
    }

    //Поиск замыкающей вершины для заданной фазы
    for(I=0;I<(*MaxLT);I++)
    {
	if((strcmp((*(*ListTop+I)).CodeTr,Cw)==0)&&((*(*ListTop+I)).Faz==(*Faza)))
        {
            Ne=I;
            break;
        }
    }

    //Определение всех ветвей параллельного подграфа, имеющего одинаковую концевую вершину
    NJ=0;
    for(I=0;I<(*MaxGf);I++)
    {
	if((*(*LstGraf+I)).NambTop==Ne)
        {
            NumP[NJ]=I;
            for(K=0;K<(*MaxLT);K++)
            {
                if((*(*ListTop+K)).FirstDef==I)
                {
                    (*(*ListTop+K)).FirstDef=-77;
                    (*(*ListTop+K)).LastDef =-77;
                    break;
                }
            }
            NJ++;
        }
    }
    //Назначение номеров процессоров параллельным веткам
    NJ=0;
    FistD=(*(*ListTop+(*FromTop))).FirstDef;
    LastD=(*(*ListTop+(*FromTop))).LastDef;
    //(*(*ListTop+(*FromTop))).F=1;
    for(FistD;FistD<=LastD;FistD++)
    {
	JT=(*(*LstGraf+FistD)).NambTop;
	(*(*ListTop+JT)).FirstDef=NumP[NJ];
	(*(*ListTop+JT)).LastDef =NumP[NJ];
	(*(*ListTop+JT)).rankT = (*rank);
	NJ++;(*rank)++;
    }
    (*FromTop)=Ne;

    //if(strcmp(Cw,"0.E")==0)
    //	(*Faza)++;

    return 1;
}
PROJECT_END_NAMESPACE
