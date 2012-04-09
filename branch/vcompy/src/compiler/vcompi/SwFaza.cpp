/*Поиск необработанных веток графаю Увеличение фазы*/
#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *,char *,char *,int *,int *);

int SwFaza(COMPTOPs *GPcomp,int *FromTop,int *MaxLT,int *root,int *FLH,int *Faza)
{
int i;
static int Nurov, Kurov;
Char200 Cod;
static char WR='R',Tip;

for(i=0;i<(*MaxLT);i++) 
	//if((*(*GPcomp+i)).rankT==0 && (*(*GPcomp+i)).F==0 && (*(*GPcomp+i)).Faz==(*Faza))
	{
	strcpy(Cod,(*(*GPcomp+i)).CodeTr);
	BcodeTr(&Cod,&WR,&Tip,&Nurov,&Kurov);
	if(Tip=='H' && (*(*GPcomp+i)).F==0 && (*(*GPcomp+i)).Faz==(*Faza))
		{
		(*FromTop)=i;
		 (*root)=i;
		 (*FLH)=1;
		 return 1;}
	}
(*Faza)++;
for(i=0;i<(*MaxLT);i++)
	{ 
	strcpy(Cod,(*(*GPcomp+i)).CodeTr);
	BcodeTr(&Cod,&WR,&Tip,&Nurov,&Kurov);
	if(Tip=='H' && (*(*GPcomp+i)).F==0 && (*(*GPcomp+i)).Faz==(*Faza))
		{(*FromTop)=i;
		 (*root)=i;
		 (*FLH)=2;
		 //(*(*GPcomp+i)).rankT=0;
		 return 1;
		}
	}
(*FLH)=0;
return 1;	
}
PROJECT_END_NAMESPACE
