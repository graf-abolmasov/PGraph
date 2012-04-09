#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int SF14128A2(COMPTOPs *ListTop, DEFGRAFs *LstGraf, Char200 *Cod, int *FromTop, int *MaxLT, int *MaxGf, int *rank, int *Faza);
int A6DFD2020(TPOData *D)
{
//Поиск концевой вершины параллельного графа
	COMPTOPs *_ListTop = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;
	Char200 *_Cod = D->CodPU;
	int *_FromTop = D->FromTop;
	int *_MaxLT = D->MaxLT;
	int *_MaxGf = D->MaxGf;
	int *_rank = D->rank;
	int *_Faza = D->Faza;

	int result = SF14128A2(_ListTop, _LstGraf, _Cod, _FromTop, _MaxLT, _MaxGf, _rank, _Faza);

	D->GPcomp = _ListTop;
	D->GPgraf = _LstGraf;
	D->CodPU = _Cod;
	D->FromTop = _FromTop;
	D->MaxLT = _MaxLT;
	D->MaxGf = _MaxGf;
	D->rank = _rank;
	D->Faza = _Faza;


	return result;
}PROJECT_END_NAMESPACE
