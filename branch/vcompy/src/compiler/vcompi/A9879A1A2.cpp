#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S371DEA77(COMPTOPs *ListTop, DEFGRAFs *LstGraf);
int A9879A1A2(TPOData *D)
{
//Наполнение массивов описания
	COMPTOPs *_ListTop = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;

	int result = S371DEA77(_ListTop, _LstGraf);

	D->GPcomp = _ListTop;
	D->GPgraf = _LstGraf;


	return result;
}PROJECT_END_NAMESPACE
