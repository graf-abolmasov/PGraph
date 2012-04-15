#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P8095298(TPOData *D);
int P1C72B9D5(TPOData *D);
int P3A844A0E(TPOData *D);
int P62843319(TPOData *D);
int P282E6F54(TPOData *D);
int AB5D3E698(TPOData *D);
int G9374E37D(TPOData *D);
int A6DFD2020(TPOData *D);
int A674A2D96(TPOData *D);
int A9E722978(TPOData *D);
int ABFAF5078(TPOData *D);
int AFC8A1A59(TPOData *D);
int A39A80DFE(TPOData *D);
int A9D803DBF(TPOData *D);
int GF0BF02CB(TPOData *D);
int AC544C50(TPOData *D);
int G3CDB9BDD(TPOData *D);
int ABE7A0F38(TPOData *D);
int ADCBC8AD7(TPOData *D);
int AEF49659B(TPOData *D);
int G848B1898(TPOData *D);
int A997A5B31(TPOData *D);
int ADF8D7A12(TPOData *D);
int AAD9D3184(TPOData *D);
int A8C6F61DD(TPOData *D);
int AFA2E8F65(TPOData *D);
int ABF380A4A(TPOData *D);
static DefinePredicate ListPred[5] = {
	DefinePredicate("P8095298", &P8095298),
	DefinePredicate("P1C72B9D5", &P1C72B9D5),
	DefinePredicate("P3A844A0E", &P3A844A0E),
	DefinePredicate("P62843319", &P62843319),
	DefinePredicate("P282E6F54", &P282E6F54)
};
static DefineTop ListTop[3] = {
	DefineTop("AFA2E8F65", 0, 0, &AFA2E8F65),
	DefineTop("A9D803DBF", 1, 1, &A9D803DBF),
	DefineTop("GF0BF02CB", -77, -77, &GF0BF02CB)
};
static DefineGraph ListGraf[2] = {
	DefineGraph(4, 1, 1),
	DefineGraph(4, 2, 1)
};
int G9374E37D(TPOData *D)
{
	//Декомпозиция параллельного графа на составляющие
	//printf("Декомпозиция параллельного графа на составляющие\r\n");
	int topCount = 0; //never used parameter
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
