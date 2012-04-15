#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P8095298(TPOData *D);
int P1C72B9D5(TPOData *D);
int P282E6F54(TPOData *D);
int AB5D3E698(TPOData *D);
int ABFAF5078(TPOData *D);
int A6DFD2020(TPOData *D);
int A674A2D96(TPOData *D);
int AFC8A1A59(TPOData *D);
int A997A5B31(TPOData *D);
int ABE7A0F38(TPOData *D);
int ABF380A4A(TPOData *D);
int AEF49659B(TPOData *D);
int AC544C50(TPOData *D);
int ADF8D7A12(TPOData *D);
int G3CDB9BDD(TPOData *D);
static DefinePredicate ListPred[3] = {
	DefinePredicate("P8095298", &P8095298),
	DefinePredicate("P1C72B9D5", &P1C72B9D5),
	DefinePredicate("P282E6F54", &P282E6F54)
};
static DefineTop ListTop[11] = {
	DefineTop("AC544C50", 0, 0, &AC544C50),
	DefineTop("AB5D3E698", 1, 1, &AB5D3E698),
	DefineTop("AEF49659B", 2, 2, &AEF49659B),
	DefineTop("ABE7A0F38", 3, 4, &ABE7A0F38),
	DefineTop("ABFAF5078", 5, 5, &ABFAF5078),
	DefineTop("A674A2D96", 6, 6, &A674A2D96),
	DefineTop("AFC8A1A59", 7, 8, &AFC8A1A59),
	DefineTop("A6DFD2020", 9, 9, &A6DFD2020),
	DefineTop("ADF8D7A12", -77, -77, &ADF8D7A12),
	DefineTop("A997A5B31", 10, 10, &A997A5B31),
	DefineTop("ABF380A4A", 11, 11, &ABF380A4A)
};
static DefineGraph ListGraf[12] = {
	DefineGraph(2, 1, 1),
	DefineGraph(2, 2, 1),
	DefineGraph(2, 3, 1),
	DefineGraph(1, 4, 1),
	DefineGraph(2, 5, 1),
	DefineGraph(2, 10, 1),
	DefineGraph(2, 6, 1),
	DefineGraph(0, 3, 1),
	DefineGraph(2, 7, 1),
	DefineGraph(2, 8, 1),
	DefineGraph(2, 6, 1),
	DefineGraph(2, 9, 1)
};
int G3CDB9BDD(TPOData *D)
{
	//Модуль агрегации ветвей параллельного подграфа
	//printf("Модуль агрегации ветвей параллельного подграфа\r\n");
	int topCount = 0; //never used parameter
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
