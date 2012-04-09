#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S89829134(COMPTOPs *ListTop, DEFGRAFs *LstGraf, int *MaxLT, Char200 *CodT, int *FlGr, int *root, int *Faza);
int AEF99357A(TPOData *D)
{
//Поиск неописанных вершин
	COMPTOPs *_ListTop = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;
	int *_MaxLT = D->MaxLT;
	Char200 *_CodT = D->CodPU;
	int *_FlGr = D->FlGr;
	int *_root = D->FromTop;
	int *_Faza = D->Faza;

	int result = S89829134(_ListTop, _LstGraf, _MaxLT, _CodT, _FlGr, _root, _Faza);

	D->GPcomp = _ListTop;
	D->GPgraf = _LstGraf;
	D->MaxLT = _MaxLT;
	D->CodPU = _CodT;
	D->FlGr = _FlGr;
	D->FromTop = _root;
	D->Faza = _Faza;


	return result;
}PROJECT_END_NAMESPACE
