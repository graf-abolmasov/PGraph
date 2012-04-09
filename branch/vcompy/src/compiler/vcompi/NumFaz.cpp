#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *Cod,char *WR,char *Tip,int *Nurov,int *Kurov); 


int NumFaz(COMPTOPs *ListTop,DEFGRAFs *LstGraf,Char200 *Cod,int *FromTop,int *Faza,int *MaxLT)
{ 
// Здесь *MaxLT - количество вершин нумеруемого графа
// *MaxLG - количество ребер нумеруемого графа
int I,R,MaxFaza,NFaz;
char WR='R',TTip;

BcodeTr(Cod,&WR,&TTip,&R,&R); 
strncpy((*(*ListTop + (*FromTop))).CodeTr,*Cod,199); 

if(TTip=='H')
{
	MaxFaza=1;NFaz=0;
	for(I=0;I<(*MaxLT);I++)
		{
		if(strcmp((*(*ListTop+I)).CodeTr,*Cod)==0)
			{
			NFaz++;
			if((*(*ListTop + I)).Faz > MaxFaza)
				MaxFaza=(*(*ListTop + I)).Faz;
			}
		}
	if(MaxFaza < NFaz)
		(*Faza)++;
}		
(*(*ListTop + (*FromTop))).Faz=(*Faza);

return 1;
}
PROJECT_END_NAMESPACE
