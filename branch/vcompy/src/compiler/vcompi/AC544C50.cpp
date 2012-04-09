#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int SEF5AFCAF(NAMAGR *NameGr, int *Nobj);
int AC544C50(TPOData *D)
{
//Формирвоание имен подграфов
	NAMAGR *_NameGr = D->NameGr;
	int *_Nobj = D->Nobj;

	int result = SEF5AFCAF(_NameGr, _Nobj);

	D->NameGr = _NameGr;
	D->Nobj = _Nobj;


	return result;
}PROJECT_END_NAMESPACE
