#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S19E6D097(Char200 *Cod, char *Tip, int *Nurov, int *Kurov);
int AC72B7C3D(TPOData *D)
{
//Чтение NurС c KurC
	Char200 *_Cod = D->CodPU;
	char *_Tip = D->TurC;
	int *_Nurov = D->NurC;
	int *_Kurov = D->KurC;

	int result = S19E6D097(_Cod, _Tip, _Nurov, _Kurov);

	D->CodPU = _Cod;
	D->TurC = _Tip;
	D->NurC = _Nurov;
	D->KurC = _Kurov;


	return result;
}PROJECT_END_NAMESPACE
