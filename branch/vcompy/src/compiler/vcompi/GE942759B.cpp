#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P3307FCC1(TPOData *D);
int P11D5122C(TPOData *D);
int PEA3294B1(TPOData *D);
int PDC5EFC2C(TPOData *D);
int P282E6F54(TPOData *D);
int PDC8C0A19(TPOData *D);
int P62B749BA(TPOData *D);
int P2EDD443B(TPOData *D);
int P3708EB64(TPOData *D);
int A52792D93(TPOData *D);
int G8231D095(TPOData *D);
int A781AE049(TPOData *D);
int GE942759B(TPOData *D);
int A81D79BB8(TPOData *D);
int A8418E7E1(TPOData *D);
int A684C9F79(TPOData *D);
int AE05016B2(TPOData *D);
int A1B2378BE(TPOData *D);
int A87DDA0B5(TPOData *D);
int AFC8A1A59(TPOData *D);
int AEF49659B(TPOData *D);
int AC72B7C3D(TPOData *D);
int A71A7531D(TPOData *D);
int A2D21EB55(TPOData *D);
int A25C2C0C3(TPOData *D);
int A92AC8659(TPOData *D);
int A21A2E4BC(TPOData *D);
int ADCBC8AD7(TPOData *D);
static DefinePredicate ListPred[9] = {
	DefinePredicate("P3307FCC1", &P3307FCC1),
	DefinePredicate("P11D5122C", &P11D5122C),
	DefinePredicate("PEA3294B1", &PEA3294B1),
	DefinePredicate("PDC5EFC2C", &PDC5EFC2C),
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("PDC8C0A19", &PDC8C0A19),
	DefinePredicate("P62B749BA", &P62B749BA),
	DefinePredicate("P2EDD443B", &P2EDD443B),
	DefinePredicate("P3708EB64", &P3708EB64)
};
static DefineTop ListTop[15] = {
	DefineTop("G8231D095", 0, 2, &G8231D095),
	DefineTop("A8418E7E1", 3, 3, &A8418E7E1),
	DefineTop("A684C9F79", 4, 4, &A684C9F79),
	DefineTop("AFC8A1A59", 5, 6, &AFC8A1A59),
	DefineTop("A71A7531D", 7, 7, &A71A7531D),
	DefineTop("AC72B7C3D", 8, 8, &AC72B7C3D),
	DefineTop("A52792D93", 9, 9, &A52792D93),
	DefineTop("A81D79BB8", 10, 10, &A81D79BB8),
	DefineTop("A25C2C0C3", 11, 11, &A25C2C0C3),
	DefineTop("A81D79BB8", 12, 12, &A81D79BB8),
	DefineTop("A92AC8659", 13, 14, &A92AC8659),
	DefineTop("A21A2E4BC", 15, 15, &A21A2E4BC),
	DefineTop("A2D21EB55", -77, -77, &A2D21EB55),
	DefineTop("A81D79BB8", 16, 16, &A81D79BB8),
	DefineTop("AE05016B2", 17, 17, &AE05016B2)
};
static DefineGraph ListGraf[18] = {
	DefineGraph(1, 1),
	DefineGraph(0, 2),
	DefineGraph(4, 3),
	DefineGraph(4, 13),
	DefineGraph(4, 8),
	DefineGraph(6, 3),
	DefineGraph(4, 4),
	DefineGraph(4, 5),
	DefineGraph(4, 6),
	DefineGraph(4, 7),
	DefineGraph(4, 10),
	DefineGraph(4, 9),
	DefineGraph(4, 10),
	DefineGraph(2, 11),
	DefineGraph(4, 12),
	DefineGraph(4, 12),
	DefineGraph(4, 10),
	DefineGraph(4, 0)
};
int GE942759B(TPOData *D)
{
	//Разметка вершин преемников для дуг типа 2
	//printf("Разметка вершин преемников для дуг типа 2\r\n");
	int topCount = 15;
	int rootTop = 14;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
