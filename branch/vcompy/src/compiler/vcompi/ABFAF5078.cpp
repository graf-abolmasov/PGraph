#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int SEF5AFCAF(NAMAGR *NameGr, int *Nobj);
int ABFAF5078(TPOData *D)
{
//Формирование имени WSP
	NAMAGR *_NameGr = D->WSP;
	int *_Nobj = D->Nobj;

	int result = SEF5AFCAF(_NameGr, _Nobj);

	D->WSP = _NameGr;
	D->Nobj = _Nobj;


	return result;
}PROJECT_END_NAMESPACE
