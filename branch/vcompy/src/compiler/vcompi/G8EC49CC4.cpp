#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int PDC5EFC2C(TPOData *D);
int P11D5122C(TPOData *D);
int P92415BF3(TPOData *D);
int P62B749BA(TPOData *D);
int PEA3294B1(TPOData *D);
int P3307FCC1(TPOData *D);
int P2EDD443B(TPOData *D);
int PDC8C0A19(TPOData *D);
int P3201D7B0(TPOData *D);
int P3869C48C(TPOData *D);
int P282E6F54(TPOData *D);
int P1DFF9165(TPOData *D);
int P6CD51F69(TPOData *D);
int P36DF27A0(TPOData *D);
int P3708EB64(TPOData *D);
int PB4337ADE(TPOData *D);
int PFF71235F(TPOData *D);
int PCD7838ED(TPOData *D);
int P538C8D9F(TPOData *D);
int P4168D9(TPOData *D);
int A87DDA0B5(TPOData *D);
int AFC8A1A59(TPOData *D);
int A951652FD(TPOData *D);
int A8C4D87AE(TPOData *D);
int AEEDFC3CC(TPOData *D);
int A2D21EB55(TPOData *D);
int A5DADAA5E(TPOData *D);
int A684C9F79(TPOData *D);
int ABE7A0F38(TPOData *D);
int A21A2E4BC(TPOData *D);
int A781AE049(TPOData *D);
int AEF99357A(TPOData *D);
int A25C2C0C3(TPOData *D);
int AE05016B2(TPOData *D);
int A5B1E6B76(TPOData *D);
int AC72B7C3D(TPOData *D);
int AB518086E(TPOData *D);
int G8EC49CC4(TPOData *D);
int GE942759B(TPOData *D);
int A289590B9(TPOData *D);
int G6C578BEC(TPOData *D);
int A92AC8659(TPOData *D);
int A52792D93(TPOData *D);
int A81D79BB8(TPOData *D);
int AEF49659B(TPOData *D);
int AFDF9CBC5(TPOData *D);
int A1B2378BE(TPOData *D);
int G8231D095(TPOData *D);
int A71A7531D(TPOData *D);
int G57D6B83A(TPOData *D);
int ADCBC8AD7(TPOData *D);
int A8418E7E1(TPOData *D);
static DefinePredicate ListPred[20] = {
	DefinePredicate("PDC5EFC2C", &PDC5EFC2C),
	DefinePredicate("P11D5122C", &P11D5122C),
	DefinePredicate("P92415BF3", &P92415BF3),
	DefinePredicate("P62B749BA", &P62B749BA),
	DefinePredicate("PEA3294B1", &PEA3294B1),
	DefinePredicate("P3307FCC1", &P3307FCC1),
	DefinePredicate("P2EDD443B", &P2EDD443B),
	DefinePredicate("PDC8C0A19", &PDC8C0A19),
	DefinePredicate("P3201D7B0", &P3201D7B0),
	DefinePredicate("P3869C48C", &P3869C48C),
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("P1DFF9165", &P1DFF9165),
	DefinePredicate("P6CD51F69", &P6CD51F69),
	DefinePredicate("P36DF27A0", &P36DF27A0),
	DefinePredicate("P3708EB64", &P3708EB64),
	DefinePredicate("PB4337ADE", &PB4337ADE),
	DefinePredicate("PFF71235F", &PFF71235F),
	DefinePredicate("PCD7838ED", &PCD7838ED),
	DefinePredicate("P538C8D9F", &P538C8D9F),
	DefinePredicate("P4168D9", &P4168D9)
};
static DefineTop ListTop[9] = {
	DefineTop("AB518086E", 0, 2, &AB518086E),
	DefineTop("G57D6B83A", 3, 3, &G57D6B83A),
	DefineTop("GE942759B", 4, 4, &GE942759B),
	DefineTop("G6C578BEC", 5, 5, &G6C578BEC),
	DefineTop("A5DADAA5E", 6, 9, &A5DADAA5E),
	DefineTop("A21A2E4BC", 10, 10, &A21A2E4BC),
	DefineTop("AFDF9CBC5", 11, 11, &AFDF9CBC5),
	DefineTop("AEF99357A", 12, 13, &AEF99357A),
	DefineTop("ADCBC8AD7", -77, -77, &ADCBC8AD7)
};
static DefineGraph ListGraf[14] = {
	DefineGraph(8, 1),
	DefineGraph(19, 2),
	DefineGraph(12, 3),
	DefineGraph(10, 4),
	DefineGraph(10, 4),
	DefineGraph(10, 4),
	DefineGraph(16, 5),
	DefineGraph(9, 6),
	DefineGraph(15, 7),
	DefineGraph(10, 0),
	DefineGraph(10, 6),
	DefineGraph(10, 7),
	DefineGraph(2, 0),
	DefineGraph(10, 8)
};
int G8EC49CC4(TPOData *D)
{
	//Линейный проход по дереву графа управления структурируемой программы
	//printf("Линейный проход по дереву графа управления структурируемой программы\r\n");
	int topCount = 9;
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
