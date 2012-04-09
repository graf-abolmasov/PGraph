#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int SEAFC33C7(COMPTOPs *ListTop, DEFGRAFs *LstGraf, int *NumTop, int *MaxLT, int *MaxLG);
int AFCD704BA(TPOData *D)
{
//Преобразование к форме ППГ в виде люстры
	COMPTOPs *_ListTop = D->GPcomp;
	DEFGRAFs *_LstGraf = D->GPgraf;
	int *_NumTop = D->NumTop;
	int *_MaxLT = D->MaxLT;
	int *_MaxLG = D->MaxGf;

	int result = SEAFC33C7(_ListTop, _LstGraf, _NumTop, _MaxLT, _MaxLG);

	D->GPcomp = _ListTop;
	D->GPgraf = _LstGraf;
	D->NumTop = _NumTop;
	D->MaxLT = _MaxLT;
	D->MaxGf = _MaxLG;


	return result;
}PROJECT_END_NAMESPACE
