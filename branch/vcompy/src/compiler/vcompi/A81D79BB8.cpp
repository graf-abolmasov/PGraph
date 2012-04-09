#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S7730E5ED(Char200 *Cod, char *WR, char *Tip, int *Nurov, int *Kurov);
int A81D79BB8(TPOData *D)
{
//Формирование текущего кода вершины для типа 2
	Char200 *_Cod = D->CodPU;
	char *_WR = D->cW;
	char *_Tip = D->cH;
	int *_Nurov = D->NurC;
	int *_Kurov = D->KurC;

	int result = S7730E5ED(_Cod, _WR, _Tip, _Nurov, _Kurov);

	D->CodPU = _Cod;
	D->cW = _WR;
	D->cH = _Tip;
	D->NurC = _Nurov;
	D->KurC = _Kurov;


	return result;
}PROJECT_END_NAMESPACE
