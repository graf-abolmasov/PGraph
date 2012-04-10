#include "utype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int print_struct(COMPTOPs *ListTop,DEFGRAFs *LstGraf, int *MaxLT, int *MaxGf);
int S6CCCAD75(COMPTOPs *ListTop,DEFGRAFs *LstGraf, int *MaxLT, int *MaxGf)
{
	return print_struct(ListTop, LstGraf, MaxLT, MaxGf);
}
PROJECT_END_NAMESPACE
