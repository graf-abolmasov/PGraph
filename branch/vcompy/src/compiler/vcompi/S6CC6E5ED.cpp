#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int BPAGR(COMPTOPs *GPcomp,Char200 *CodPU,int *FistD, int *I0,int *MaxLT,int *Faza);
int S6CC6E5ED(COMPTOPs *GPcomp,Char200 *CodPU,int *FistD, int *I0,int *MaxLT,int *Faza)
{
	return BPAGR(GPcomp, CodPU, FistD, I0, MaxLT, Faza);
}
PROJECT_END_NAMESPACE
