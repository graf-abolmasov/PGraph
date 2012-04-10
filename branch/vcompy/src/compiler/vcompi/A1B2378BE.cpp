#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S7941FB6A(COMPTOPs *ListTop, DEFGRAFs *LstGraf, Char200 *Cod, int *FromTop, int *Faza, int *MaxLT);
int A1B2378BE(TPOData *D)
{
//Нумерация фаз параллельных компонент графа
	COMPTOPs *_ListTop = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;
	Char200 *_Cod = D->CodPU;
	int *_FromTop = D->FromTop;
	int *_Faza = D->Faza;
	int *_MaxLT = D->MaxLT;

	int result = S7941FB6A(_ListTop, _LstGraf, _Cod, _FromTop, _Faza, _MaxLT);

	D->GPcomp = _ListTop;
	D->GPgraf = _LstGraf;
	D->CodPU = _Cod;
	D->FromTop = _FromTop;
	D->Faza = _Faza;
	D->MaxLT = _MaxLT;

	return result;
}PROJECT_END_NAMESPACE
