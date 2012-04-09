#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S19086399(COMPTOPs *GPcomp, int *FromTop, int *MaxLT, int *root, int *FLH, int *Faza);
int A9E722978(TPOData *D)
{
//Поиск необработанных вершин размеченного графа
	COMPTOPs *_GPcomp = D->GPcomp;
	int *_FromTop = D->FromTop;
	int *_MaxLT = D->MaxLT;
	int *_root = D->root;
	int *_FLH = D->FLH;
	int *_Faza = D->Faza;

	int result = S19086399(_GPcomp, _FromTop, _MaxLT, _root, _FLH, _Faza);

	D->GPcomp = _GPcomp;
	D->FromTop = _FromTop;
	D->MaxLT = _MaxLT;
	D->root = _root;
	D->FLH = _FLH;
	D->Faza = _Faza;


	return result;
}PROJECT_END_NAMESPACE
