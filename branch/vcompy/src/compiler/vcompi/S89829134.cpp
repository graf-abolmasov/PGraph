#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int FullTr(COMPTOPs *ListTop, DEFGRAFs *LstGraf, int *MaxLT, Char200 *CodT, int *FlGr, int *root, int *Faza);
int S89829134(COMPTOPs *ListTop, DEFGRAFs *LstGraf, int *MaxLT, Char200 *CodT, int *FlGr, int *root, int *Faza)
{
	return FullTr(ListTop, LstGraf, MaxLT, CodT, FlGr, root, Faza);
}
PROJECT_END_NAMESPACE
