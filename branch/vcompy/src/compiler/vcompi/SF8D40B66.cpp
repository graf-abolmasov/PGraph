#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int BFLH(COMPTOPs *GPcomp,DEFGRAFs *LstGraf,int *FromTop, int *FLH,int *MaxLT,int *Faza);
int SF8D40B66(COMPTOPs *GPcomp,DEFGRAFs *LstGraf,int *FromTop, int *FLH,int *MaxLT,int *Faza)
{
	return BFLH(GPcomp, LstGraf, FromTop, FLH, MaxLT, Faza);
}
PROJECT_END_NAMESPACE
