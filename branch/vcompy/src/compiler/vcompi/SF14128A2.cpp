#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int BfindE(COMPTOPs *ListTop,DEFGRAFs *LstGraf,Char200 *Cod,int *FromTop,int *MaxLT,int *MaxGf,int *rank,int *Faza);
int SF14128A2(COMPTOPs *ListTop,DEFGRAFs *LstGraf,Char200 *Cod,int *FromTop,int *MaxLT,int *MaxGf,int *rank,int *Faza)
{
    return BfindE(ListTop, LstGraf, Cod, FromTop, MaxLT, MaxGf, rank, Faza);
}
PROJECT_END_NAMESPACE
