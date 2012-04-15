#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P36DF27A0(TPOData *D);
int P1DFF9165(TPOData *D);
int P3307FCC1(TPOData *D);
int P3708EB64(TPOData *D);
int P2EDD443B(TPOData *D);
int P62B749BA(TPOData *D);
int PDC5EFC2C(TPOData *D);
int PEA3294B1(TPOData *D);
int P282E6F54(TPOData *D);
int PDC8C0A19(TPOData *D);
int A1B2378BE(TPOData *D);
int G8231D095(TPOData *D);
int A81D79BB8(TPOData *D);
int AFC8A1A59(TPOData *D);
int A781AE049(TPOData *D);
int ABE7A0F38(TPOData *D);
int AC72B7C3D(TPOData *D);
int AEF49659B(TPOData *D);
int A684C9F79(TPOData *D);
int A92AC8659(TPOData *D);
int A21A2E4BC(TPOData *D);
int A87DDA0B5(TPOData *D);
int ADCBC8AD7(TPOData *D);
int G57D6B83A(TPOData *D);
int A2D21EB55(TPOData *D);
static DefinePredicate ListPred[10] = {
	DefinePredicate("P36DF27A0", &P36DF27A0),
	DefinePredicate("P1DFF9165", &P1DFF9165),
	DefinePredicate("P3307FCC1", &P3307FCC1),
	DefinePredicate("P3708EB64", &P3708EB64),
	DefinePredicate("P2EDD443B", &P2EDD443B),
	DefinePredicate("P62B749BA", &P62B749BA),
	DefinePredicate("PDC5EFC2C", &PDC5EFC2C),
	DefinePredicate("PEA3294B1", &PEA3294B1),
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("PDC8C0A19", &PDC8C0A19)
};
static DefineTop ListTop[12] = {
	DefineTop("G8231D095", 0, 2, &G8231D095),
	DefineTop("A684C9F79", 3, 3, &A684C9F79),
	DefineTop("A87DDA0B5", 4, 4, &A87DDA0B5),
	DefineTop("A92AC8659", 5, 6, &A92AC8659),
	DefineTop("A21A2E4BC", 7, 7, &A21A2E4BC),
	DefineTop("A2D21EB55", -77, -77, &A2D21EB55),
	DefineTop("GHOST TOP", -77, -77, NULL),
	DefineTop("GHOST TOP", -77, -77, NULL),
	DefineTop("AFC8A1A59", 8, 9, &AFC8A1A59),
	DefineTop("A92AC8659", -77, -77, &A92AC8659),
	DefineTop("ABE7A0F38", 10, 11, &ABE7A0F38),
	DefineTop("ABE7A0F38", 12, 12, &ABE7A0F38)
};
static DefineGraph ListGraf[13] = {
	DefineGraph(1, 9, 1),
	DefineGraph(2, 1, 1),
	DefineGraph(8, 10, 1),
	DefineGraph(8, 2, 1),
	DefineGraph(8, 3, 1),
	DefineGraph(7, 4, 1),
	DefineGraph(8, 5, 1),
	DefineGraph(8, 5, 1),
	DefineGraph(5, 8, 1),
	DefineGraph(8, 11, 1),
	DefineGraph(0, 8, 1),
	DefineGraph(8, 11, 1),
	DefineGraph(8, 3, 1)
};
int G57D6B83A(TPOData *D)
{
	//Разметка вершин преемников типа 1
	//printf("Разметка вершин преемников типа 1\r\n");
	int topCount = 0; //never used parameter
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
