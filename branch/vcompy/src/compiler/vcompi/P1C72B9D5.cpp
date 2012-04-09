#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S6CC6E5ED(COMPTOPs *GPcomp, Char200 *CodPU, int *FistD, int *I0, int *MaxLT, int *Faza);
int P1C72B9D5(TPOData *D)
{
//Проверка необходимости агрегирования модулей параллельной ветки
	COMPTOPs *_GPcomp = D->GPcomp;
	Char200 *_CodPU = D->CodPU;
	int *_FistD = D->FistD;
	int *_I0 = D->I;
	int *_MaxLT = D->MaxLT;
	int *_Faza = D->Faza;

	return S6CC6E5ED(_GPcomp, _CodPU, _FistD, _I0, _MaxLT, _Faza);

}
PROJECT_END_NAMESPACE
