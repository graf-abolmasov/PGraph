#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P282E6F54(TPOData *D);
int PEA3294B1(TPOData *D);
int P3307FCC1(TPOData *D);
int PDC5EFC2C(TPOData *D);
int P2EDD443B(TPOData *D);
int PDC8C0A19(TPOData *D);
int P3708EB64(TPOData *D);
int A81D79BB8(TPOData *D);
int A684C9F79(TPOData *D);
int A21A2E4BC(TPOData *D);
int A87DDA0B5(TPOData *D);
int AC72B7C3D(TPOData *D);
int A1B2378BE(TPOData *D);
int A781AE049(TPOData *D);
int G8231D095(TPOData *D);
int ADCBC8AD7(TPOData *D);
int AEF49659B(TPOData *D);
static DefinePredicate ListPred[7] = {
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("PEA3294B1", &PEA3294B1),
	DefinePredicate("P3307FCC1", &P3307FCC1),
	DefinePredicate("PDC5EFC2C", &PDC5EFC2C),
	DefinePredicate("P2EDD443B", &P2EDD443B),
	DefinePredicate("PDC8C0A19", &PDC8C0A19),
	DefinePredicate("P3708EB64", &P3708EB64)
};
static DefineTop ListTop[10] = {
	DefineTop("AEF49659B", 0, 1, &AEF49659B),
	DefineTop("ADCBC8AD7", -77, -77, &ADCBC8AD7),
	DefineTop("A781AE049", -77, -77, &A781AE049),
	DefineTop("AC72B7C3D", 2, 3, &AC72B7C3D),
	DefineTop("A684C9F79", 4, 6, &A684C9F79),
	DefineTop("A81D79BB8", 7, 7, &A81D79BB8),
	DefineTop("A87DDA0B5", 8, 8, &A87DDA0B5),
	DefineTop("A87DDA0B5", 9, 10, &A87DDA0B5),
	DefineTop("A21A2E4BC", 11, 11, &A21A2E4BC),
	DefineTop("A1B2378BE", -77, -77, &A1B2378BE)
};
static DefineGraph ListGraf[12] = {
	DefineGraph(2, 3),
	DefineGraph(0, 1),
	DefineGraph(3, 4),
	DefineGraph(0, 2),
	DefineGraph(6, 5),
	DefineGraph(4, 6),
	DefineGraph(5, 7),
	DefineGraph(0, 9),
	DefineGraph(0, 9),
	DefineGraph(1, 8),
	DefineGraph(0, 9),
	DefineGraph(0, 9)
};
int G8231D095(TPOData *D)
{
	//Маркировка вершин таблицы GPcomp
	//printf("Маркировка вершин таблицы GPcomp\r\n");
	int topCount = 10;
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
