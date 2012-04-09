#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S1313BA02(COMPTOPs *GPcomp, DEFGRAFs *GPgraf, int *I, NAMAGR *WSP, NAMAGR *NameGr, int *FromTop, int *MaxLT);
int ABF380A4A(TPOData *D)
{
//Добавление новых вершин в массив GPcomp
	COMPTOPs *_GPcomp = D->GPcomp;
	DEFGRAFs *_GPgraf = D->GPgraf;
	int *_I = D->I;
	NAMAGR *_WSP = D->WSP;
	NAMAGR *_NameGr = D->NameGr;
	int *_FromTop = D->FromTop;
	int *_MaxLT = D->MaxLT;

	int result = S1313BA02(_GPcomp, _GPgraf, _I, _WSP, _NameGr, _FromTop, _MaxLT);

	D->GPcomp = _GPcomp;
	D->GPgraf = _GPgraf;
	D->I = _I;
	D->WSP = _WSP;
	D->NameGr = _NameGr;
	D->FromTop = _FromTop;
	D->MaxLT = _MaxLT;


	return result;
}PROJECT_END_NAMESPACE
