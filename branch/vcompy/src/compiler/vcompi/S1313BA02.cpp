#include "utype.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int AddTop(COMPTOPs *GPcomp,DEFGRAFs *GPgraf,int *I,NAMAGR *WSP,NAMAGR *NameGr,int *FromTop,int *MaxLT);
int S1313BA02(COMPTOPs *GPcomp,DEFGRAFs *GPgraf,int *I,NAMAGR *WSP,NAMAGR *NameGr,int *FromTop,int *MaxLT)
{
	return AddTop(GPcomp, GPgraf, I, WSP, NameGr, FromTop, MaxLT);
}
PROJECT_END_NAMESPACE
