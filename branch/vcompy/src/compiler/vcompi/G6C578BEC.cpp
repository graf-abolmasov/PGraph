#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P3307FCC1(TPOData *D);
int PDC5EFC2C(TPOData *D);
int PEA3294B1(TPOData *D);
int P282E6F54(TPOData *D);
int P538C8D9F(TPOData *D);
int PDC8C0A19(TPOData *D);
int P2EDD443B(TPOData *D);
int PCD7838ED(TPOData *D);
int P3708EB64(TPOData *D);
int G8231D095(TPOData *D);
int A951652FD(TPOData *D);
int A781AE049(TPOData *D);
int A81D79BB8(TPOData *D);
int A684C9F79(TPOData *D);
int A1B2378BE(TPOData *D);
int A87DDA0B5(TPOData *D);
int AEEDFC3CC(TPOData *D);
int G6C578BEC(TPOData *D);
int A8C4D87AE(TPOData *D);
int AEF49659B(TPOData *D);
int AC72B7C3D(TPOData *D);
int A2D21EB55(TPOData *D);
int A92AC8659(TPOData *D);
int A21A2E4BC(TPOData *D);
int ADCBC8AD7(TPOData *D);
int A289590B9(TPOData *D);
int A5B1E6B76(TPOData *D);
static DefinePredicate ListPred[9] = {
	DefinePredicate("P3307FCC1", &P3307FCC1),
	DefinePredicate("PDC5EFC2C", &PDC5EFC2C),
	DefinePredicate("PEA3294B1", &PEA3294B1),
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("P538C8D9F", &P538C8D9F),
	DefinePredicate("PDC8C0A19", &PDC8C0A19),
	DefinePredicate("P2EDD443B", &P2EDD443B),
	DefinePredicate("PCD7838ED", &PCD7838ED),
	DefinePredicate("P3708EB64", &P3708EB64)
};
static DefineTop ListTop[9] = {
	DefineTop("G8231D095", 0, 1, &G8231D095),
	DefineTop("AEEDFC3CC", 2, 2, &AEEDFC3CC),
	DefineTop("A684C9F79", 3, 3, &A684C9F79),
	DefineTop("A5B1E6B76", 4, 4, &A5B1E6B76),
	DefineTop("A951652FD", 5, 6, &A951652FD),
	DefineTop("A8C4D87AE", 7, 7, &A8C4D87AE),
	DefineTop("A92AC8659", 8, 8, &A92AC8659),
	DefineTop("A2D21EB55", -77, -77, &A2D21EB55),
	DefineTop("A289590B9", -77, -77, &A289590B9)
};
static DefineGraph ListGraf[9] = {
	DefineGraph(4, 8),
	DefineGraph(0, 1),
	DefineGraph(3, 2),
	DefineGraph(3, 3),
	DefineGraph(3, 4),
	DefineGraph(7, 5),
	DefineGraph(3, 6),
	DefineGraph(3, 6),
	DefineGraph(3, 7)
};
int G6C578BEC(TPOData *D)
{
	//Разметка вершин преемников для дуг типа 3
	//printf("Разметка вершин преемников для дуг типа 3\r\n");
	int topCount = 9;
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
