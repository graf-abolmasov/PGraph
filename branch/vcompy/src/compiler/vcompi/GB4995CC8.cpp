#include "graph.h"
PROJECT_BEGIN_NAMESPACE
int P43C2377C(TPOData *D);
int P6CD51F69(TPOData *D);
int P1C72B9D5(TPOData *D);
int PCD7838ED(TPOData *D);
int P36DF27A0(TPOData *D);
int P8095298(TPOData *D);
int PEA3294B1(TPOData *D);
int P1DFF9165(TPOData *D);
int P282E6F54(TPOData *D);
int P538C8D9F(TPOData *D);
int PFF71235F(TPOData *D);
int P3708EB64(TPOData *D);
int P3307FCC1(TPOData *D);
int P3869C48C(TPOData *D);
int P3201D7B0(TPOData *D);
int P3A844A0E(TPOData *D);
int PB4337ADE(TPOData *D);
int P62843319(TPOData *D);
int P2EDD443B(TPOData *D);
int P11D5122C(TPOData *D);
int P92415BF3(TPOData *D);
int P4168D9(TPOData *D);
int P62B749BA(TPOData *D);
int PDC5EFC2C(TPOData *D);
int PDC8C0A19(TPOData *D);
int ADCBC8AD7(TPOData *D);
int A684C9F79(TPOData *D);
int AB5D3E698(TPOData *D);
int A9879A1A2(TPOData *D);
int G6C578BEC(TPOData *D);
int A781AE049(TPOData *D);
int G8EC49CC4(TPOData *D);
int A674A2D96(TPOData *D);
int G57D6B83A(TPOData *D);
int AEEDFC3CC(TPOData *D);
int A951652FD(TPOData *D);
int AFCD704BA(TPOData *D);
int A289590B9(TPOData *D);
int AC72B7C3D(TPOData *D);
int A8C95E239(TPOData *D);
int A6DFD2020(TPOData *D);
int AE05016B2(TPOData *D);
int A52792D93(TPOData *D);
int G8231D095(TPOData *D);
int GB4995CC8(TPOData *D);
int AEA8362EB(TPOData *D);
int AC544C50(TPOData *D);
int ABE7A0F38(TPOData *D);
int A81D79BB8(TPOData *D);
int AEF49659B(TPOData *D);
int A21A2E4BC(TPOData *D);
int ABF380A4A(TPOData *D);
int A5DADAA5E(TPOData *D);
int G9374E37D(TPOData *D);
int GF0BF02CB(TPOData *D);
int AAD9D3184(TPOData *D);
int A25C2C0C3(TPOData *D);
int ADF8D7A12(TPOData *D);
int AB518086E(TPOData *D);
int A997A5B31(TPOData *D);
int AEF99357A(TPOData *D);
int AFC8A1A59(TPOData *D);
int A8418E7E1(TPOData *D);
int A8C6F61DD(TPOData *D);
int ABFAF5078(TPOData *D);
int A5B1E6B76(TPOData *D);
int GE942759B(TPOData *D);
int A9D803DBF(TPOData *D);
int A1B2378BE(TPOData *D);
int AC20E2D5E(TPOData *D);
int AFA2E8F65(TPOData *D);
int G848B1898(TPOData *D);
int A92AC8659(TPOData *D);
int G3CDB9BDD(TPOData *D);
int AFDF9CBC5(TPOData *D);
int A71A7531D(TPOData *D);
int A39A80DFE(TPOData *D);
int A8C4D87AE(TPOData *D);
int A87DDA0B5(TPOData *D);
int A2D21EB55(TPOData *D);
int A9E722978(TPOData *D);
static DefinePredicate ListPred[25] = {
	DefinePredicate("P43C2377C", &P43C2377C),
	DefinePredicate("P6CD51F69", &P6CD51F69),
	DefinePredicate("P1C72B9D5", &P1C72B9D5),
	DefinePredicate("PCD7838ED", &PCD7838ED),
	DefinePredicate("P36DF27A0", &P36DF27A0),
	DefinePredicate("P8095298", &P8095298),
	DefinePredicate("PEA3294B1", &PEA3294B1),
	DefinePredicate("P1DFF9165", &P1DFF9165),
	DefinePredicate("P282E6F54", &P282E6F54),
	DefinePredicate("P538C8D9F", &P538C8D9F),
	DefinePredicate("PFF71235F", &PFF71235F),
	DefinePredicate("P3708EB64", &P3708EB64),
	DefinePredicate("P3307FCC1", &P3307FCC1),
	DefinePredicate("P3869C48C", &P3869C48C),
	DefinePredicate("P3201D7B0", &P3201D7B0),
	DefinePredicate("P3A844A0E", &P3A844A0E),
	DefinePredicate("PB4337ADE", &PB4337ADE),
	DefinePredicate("P62843319", &P62843319),
	DefinePredicate("P2EDD443B", &P2EDD443B),
	DefinePredicate("P11D5122C", &P11D5122C),
	DefinePredicate("P92415BF3", &P92415BF3),
	DefinePredicate("P4168D9", &P4168D9),
	DefinePredicate("P62B749BA", &P62B749BA),
	DefinePredicate("PDC5EFC2C", &PDC5EFC2C),
	DefinePredicate("PDC8C0A19", &PDC8C0A19)
};
static DefineTop ListTop[11] = {
	DefineTop("A9879A1A2", 0, 0, &A9879A1A2),
	DefineTop("A9D803DBF", 1, 1, &A9D803DBF),
	DefineTop("G8EC49CC4", 2, 3, &G8EC49CC4),
	DefineTop("GHOST TOP", -77, -77, NULL),
	DefineTop("G9374E37D", 4, 4, &G9374E37D),
	DefineTop("AFCD704BA", 5, 5, &AFCD704BA),
	DefineTop("A8C95E239", 6, 6, &A8C95E239),
	DefineTop("AC20E2D5E", -77, -77, &AC20E2D5E),
	DefineTop("AEA8362EB", 7, 7, &AEA8362EB),
	DefineTop("A8C95E239", 8, 8, &A8C95E239),
	DefineTop("A8C95E239", 9, 9, &A8C95E239)
};
static DefineGraph ListGraf[10] = {
	DefineGraph(8, 8, 1),
	DefineGraph(8, 2, 1),
	DefineGraph(0, 9, 1),
	DefineGraph(8, 7, 1),
	DefineGraph(8, 10, 1),
	DefineGraph(8, 7, 1),
	DefineGraph(8, 1, 1),
	DefineGraph(8, 6, 1),
	DefineGraph(8, 4, 1),
	DefineGraph(8, 5, 1)
};
int GB4995CC8(TPOData *D)
{
	//Разметка параллельного графа
	//printf("Разметка параллельного графа\r\n");
	int topCount = 0; //never used parameter
	int rootTop = 0;
	GraphMV(D, rootTop, topCount, ListPred, ListTop, ListGraf);
	return 1;
}
PROJECT_END_NAMESPACE
