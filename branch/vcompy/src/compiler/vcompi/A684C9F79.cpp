#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S7730E5ED(Char200 *Cod, char *WR, char *Tip, int *Nurov, int *Kurov);
int A684C9F79(TPOData *D)
{
//Чтение кода вершины
	Char200 *_Cod = D->CodPU;
	char *_WR = D->cR;
	char *_Tip = D->TurC;
	int *_Nurov = D->NurC;
	int *_Kurov = D->KurC;

	int result = S7730E5ED(_Cod, _WR, _Tip, _Nurov, _Kurov);

	D->CodPU = _Cod;
	D->cR = _WR;
	D->TurC = _Tip;
	D->NurC = _Nurov;
	D->KurC = _Kurov;


	return result;
}PROJECT_END_NAMESPACE
