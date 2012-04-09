#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *,char *,char *,int *,int *);
int ReadTr(Char200 *,char *,int *,int *);

int BFLH(COMPTOPs *GPcomp,DEFGRAFs *LstGraf,int *FromTop, int *FLH,int *MaxLT,int *Faza)
{
int I,J,NV,FistD,LastD,NurC,KurC;
Char200 CodPU,CodP,CodH,CodV;
char WR='W',cH='H',cV='V',TurC;

FistD=(*(*GPcomp+(*FromTop))).FirstDef; 
LastD=(*(*GPcomp+(*FromTop))).LastDef;
strncpy(CodPU,(*(*GPcomp+(*FromTop))).CodeTr,199);

J=1; *FLH=0;
ReadTr(&CodPU,&TurC,&NurC,&KurC); 
strncpy(CodH,CodPU,199);
strncpy(CodV,CodPU,199);
NurC++;
for(FistD;FistD <= LastD;FistD++)
	{
	NV=0;
	KurC=J;
	strncpy(CodH,CodPU,199);
	strncpy(CodV,CodPU,199);
	BcodeTr(&CodH,&WR,&cH,&NurC,&KurC);
	strncpy(CodV,CodPU,199);
	BcodeTr(&CodV,&WR,&cV,&NurC,&KurC);
	for(I=0;I<(*MaxLT);I++)
		{
		strncpy(CodP,(*(*GPcomp+I)).CodeTr,199);
		if((strcmp(CodP,CodH)==0) && ((*(*GPcomp+I)).Faz==(*Faza))) 
			{
			*FLH=1; return 1;
			}
		if((strcmp(CodP,CodV)==0) && ((*(*GPcomp+I)).Faz==(*Faza))) 
			NV++;
		}
	if(NV>1)
		{
		*FLH=1; return 1;
		}
	J++;
	}

return 1;	
}
PROJECT_END_NAMESPACE
