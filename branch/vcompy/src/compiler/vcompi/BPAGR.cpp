/*Предикат определения необходимости агрегировать ветвь параллельной структуры*/
#include "utype.h"
#include <stdio.h>
#include <string.h>

PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *,char *,char *,int *,int *);
int ReadTr(Char200 *,char *,int *,int *);

int BPAGR(COMPTOPs *GPcomp,Char200 *CodPU,int *FistD, int *I0,int *MaxLT,int *Faza)
{
    int I,J,NV,NurC,KurC;
    Char200 CodP,CodH,CodV;
    char WR='W',cH='H',cV='V',TurC;

    J=(*I0)-(*FistD)+1;

    ReadTr(CodPU,&TurC,&NurC,&KurC);
    strncpy(CodH,(const char *)CodPU,199);
    strncpy(CodV,(const char *)CodPU,199);
    NurC++;

    NV=0;
    KurC=J;
    BcodeTr(&CodH,&WR,&cH,&NurC,&KurC);
    BcodeTr(&CodV,&WR,&cV,&NurC,&KurC);

    for(I=0;I<(*MaxLT);I++)
    {
	strncpy(CodP,(*(*GPcomp+I)).CodeTr,199);
	if((strcmp(CodP,CodH)==0) && ((*(*GPcomp+I)).Faz==(*Faza))) 
        {
            return 1;
        }
	if((strcmp(CodP,CodV)==0) && ((*(*GPcomp+I)).Faz==(*Faza))) 
            NV++;
    }
    if(NV>1)
    {
	return 1;
    }

    return 0;
}
PROJECT_END_NAMESPACE
