#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S6CCCAD75(COMPTOPs *ListTop, DEFGRAFs *LstGraf, int *MaxLT, int *MaxGf);
int AC20E2D5E(TPOData *D)
{
//Печать конечной структуры
	COMPTOPs *_ListTop = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;
	int *_MaxLT = D->MaxLT;
	int *_MaxGf = D->MaxGf;

	int result = S6CCCAD75(_ListTop, _LstGraf, _MaxLT, _MaxGf);

	D->GPcomp = _ListTop;
	D->GPgraf = _LstGraf;
	D->MaxLT = _MaxLT;
	D->MaxGf = _MaxGf;


	return result;
}PROJECT_END_NAMESPACE
