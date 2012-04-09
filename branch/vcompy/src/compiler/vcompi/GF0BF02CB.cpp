#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P282E6F54(TPOData *D);
int P1C72B9D5(TPOData *D);
int P8095298(TPOData *D);
int P62843319(TPOData *D);
int P3A844A0E(TPOData *D);
int AFC8A1A59(TPOData *D);
int AB5D3E698(TPOData *D);
int GF0BF02CB(TPOData *D);
int A39A80DFE(TPOData *D);
int AAD9D3184(TPOData *D);
int ABE7A0F38(TPOData *D);
int ADF8D7A12(TPOData *D);
int A997A5B31(TPOData *D);
int A674A2D96(TPOData *D);
int A8C6F61DD(TPOData *D);
int G848B1898(TPOData *D);
int A9E722978(TPOData *D);
int ABFAF5078(TPOData *D);
int AEF49659B(TPOData *D);
int AC544C50(TPOData *D);
int A6DFD2020(TPOData *D);
int ABF380A4A(TPOData *D);
int G3CDB9BDD(TPOData *D);
int ADCBC8AD7(TPOData *D);
static DefinePredicate ListPred[5] = {
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("P1C72B9D5", &P1C72B9D5),
	DefinePredicate("P8095298", &P8095298),
	DefinePredicate("P62843319", &P62843319),
	DefinePredicate("P3A844A0E", &P3A844A0E)
};
static DefineTop ListTop[7] = {
	DefineTop("AAD9D3184", 0, 0, &AAD9D3184),
	DefineTop("A9E722978", 1, 1, &A9E722978),
	DefineTop("A39A80DFE", 2, 3, &A39A80DFE),
	DefineTop("G3CDB9BDD", 4, 4, &G3CDB9BDD),
	DefineTop("G848B1898", 5, 5, &G848B1898),
	DefineTop("A9E722978", 6, 7, &A9E722978),
	DefineTop("ADCBC8AD7", -77, -77, &ADCBC8AD7)
};
static DefineGraph ListGraf[8] = {
	DefineGraph(0, 2),
	DefineGraph(0, 0),
	DefineGraph(3, 3),
	DefineGraph(0, 4),
	DefineGraph(0, 5),
	DefineGraph(0, 5),
	DefineGraph(4, 0),
	DefineGraph(0, 6)
};
int GF0BF02CB(TPOData *D)
{
	//Головная программа десуперпозиции графа
	//printf("Головная программа десуперпозиции графа\r\n");
	int topCount = 7;
	int rootTop = 1;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
