#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int SwFaza(COMPTOPs *GPcomp,int *FromTop,int *MaxLT,int *root,int *FLH,int *Faza);
int S19086399(COMPTOPs *GPcomp,int *FromTop,int *MaxLT,int *root,int *FLH,int *Faza)
{
	return SwFaza(GPcomp, FromTop, MaxLT, root, FLH, Faza);
}
PROJECT_END_NAMESPACE
