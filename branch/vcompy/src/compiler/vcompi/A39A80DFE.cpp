#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int SF8D40B66(COMPTOPs *GPcomp, DEFGRAFs *LstGraf, int *FromTop, int *FLH, int *MaxLT, int *Faza);
int A39A80DFE(TPOData *D)
{
//Проверка необходимости декомпозиции агрегата
	COMPTOPs *_GPcomp = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;
	int *_FromTop = D->FromTop;
	int *_FLH = D->FLH;
	int *_MaxLT = D->MaxLT;
	int *_Faza = D->Faza;

	int result = SF8D40B66(_GPcomp, _LstGraf, _FromTop, _FLH, _MaxLT, _Faza);

	D->GPcomp = _GPcomp;
	D->GPgraf = _LstGraf;
	D->FromTop = _FromTop;
	D->FLH = _FLH;
	D->MaxLT = _MaxLT;
	D->Faza = _Faza;


	return result;
}PROJECT_END_NAMESPACE
