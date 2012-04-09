#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int NumFaz(COMPTOPs *ListTop,DEFGRAFs *LstGraf,Char200 *Cod,int *FromTop,int *Faza,int *MaxLT);
int S7941FB6A(COMPTOPs *ListTop,DEFGRAFs *LstGraf,Char200 *Cod,int *FromTop,int *Faza,int *MaxLT)
{
	return NumFaz(ListTop, LstGraf, Cod, FromTop, Faza, MaxLT);
}
PROJECT_END_NAMESPACE
