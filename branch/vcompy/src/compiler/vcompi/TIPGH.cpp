/*Предикат определения является ли граф параллельным*/
#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
int BcodeTr(Char200 *,char *,char *,int *,int *);

int TIPGH(COMPTOPs *GPcomp,int *MaxLT)
{
int I,Nurov,Kurov;
Char200 Cod;
char cR='R', Tip;

for(I=0;I<(*MaxLT);I++)
	{
	strcpy(Cod,(*(*GPcomp+I)).CodeTr);
    BcodeTr(&Cod,&cR,&Tip,&Nurov,&Kurov);
	if(Tip=='H') 
		return 1;		

	}


return 0;	
}
PROJECT_END_NAMESPACE
