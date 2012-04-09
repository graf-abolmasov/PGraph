#include "tpodata.h"
#include <string.h>

PROJECT_BEGIN_NAMESPACE
TPOData::TPOData(int rank)
{
myRank = rank;
initMemory();
}

TPOData::~TPOData()
{
if (ArcType != NULL)
	delete ArcType;
if (CodPU != NULL)
	delete CodPU;
if (CodT != NULL)
	delete CodT;
if (F1go != NULL)
	delete F1go;
if (FL != NULL)
	delete FL;
if (FLH != NULL)
	delete FLH;
if (Faza != NULL)
	delete Faza;
if (FistD != NULL)
	delete FistD;
if (FlGr != NULL)
	delete FlGr;
if (FromLst != NULL)
	delete FromLst;
if (FromTop != NULL)
	delete FromTop;
if (GPcomp != NULL)
	delete GPcomp;
if (GPgraf != NULL)
	delete GPgraf;
if (HeadGH != NULL)
	delete HeadGH;
if (HeadGR != NULL)
	delete HeadGR;
if (HeadTop != NULL)
	delete HeadTop;
if (I != NULL)
	delete I;
if (KurC != NULL)
	delete KurC;
if (LastD != NULL)
	delete LastD;
if (MaxGf != NULL)
	delete MaxGf;
if (MaxLT != NULL)
	delete MaxLT;
if (NPred != NULL)
	delete NPred;
if (NTop != NULL)
	delete NTop;
if (NameGr != NULL)
	delete NameGr;
if (Nobj != NULL)
	delete Nobj;
if (NumTop != NULL)
	delete NumTop;
if (NurC != NULL)
	delete NurC;
if (ToTop != NULL)
	delete ToTop;
if (TurC != NULL)
	delete TurC;
if (WSP != NULL)
	delete WSP;
if (cE != NULL)
	delete cE;
if (cH != NULL)
	delete cH;
if (cR != NULL)
	delete cR;
if (cV != NULL)
	delete cV;
if (cW != NULL)
	delete cW;
if (final_graph != NULL)
	delete final_graph;
if (initial_graph != NULL)
	delete initial_graph;
if (rank != NULL)
	delete rank;
if (root != NULL)
	delete root;
if (root0 != NULL)
	delete root0;

}

void TPOData::initMemory()
{
if (myRank == 0) {
ArcType = NULL;
CodPU = NULL;
CodT = NULL;
F1go = NULL;
FL = NULL;
FLH = NULL;
Faza = NULL;
FistD = NULL;
FlGr = NULL;
FromLst = NULL;
FromTop = NULL;
GPcomp = NULL;
GPgraf = NULL;
HeadGH = NULL;
HeadGR = NULL;
HeadTop = NULL;
I = NULL;
KurC = NULL;
LastD = NULL;
MaxGf = NULL;
MaxLT = NULL;
NPred = NULL;
NTop = NULL;
NameGr = NULL;
Nobj = NULL;
NumTop = NULL;
NurC = NULL;
ToTop = NULL;
TurC = NULL;
WSP = NULL;
cE = NULL;
cH = NULL;
cR = NULL;
cV = NULL;
cW = NULL;
final_graph = NULL;
initial_graph = NULL;
rank = NULL;
root = NULL;
root0 = NULL;
} else {
*(ArcType = new int) = 0;
(CodPU = (Char200*)new Char200);
(CodT = (Char200*)new Char200);
*(F1go = new int) = 1;
*(FL = new int) = 0;
*(FLH = new int) = 0;
*(Faza = new int) = 1;
*(FistD = new int) = 0;
*(FlGr = new int) = 1;
*(FromLst = new int) = 0;
*(FromTop = new int) = 0;
*(GPcomp = new COMPTOPs);
*(GPgraf = new DEFGRAFs);
*(HeadGH = new int) = 0;
*(HeadGR = new int) = 0;
*(HeadTop = new int) = 0;
*(I = new int) = 0;
*(KurC = new int) = 0;
*(LastD = new int) = 0;
*(MaxGf = new int) = 29;
*(MaxLT = new int) = 21;
*(NPred = new int) = 0;
*(NTop = new int) = 0;
(NameGr = (NAMAGR*)new NAMAGR);
*(Nobj = new int) = 0;
*(NumTop = new int) = 21;
*(NurC = new int) = 0;
*(ToTop = new int) = 0;
*(TurC = new char) = 'V';
(WSP = (NAMAGR*)new NAMAGR);
*(cE = new char) = 'E';
*(cH = new char) = 'H';
*(cR = new char) = 'R';
*(cV = new char) = 'V';
*(cW = new char) = 'W';
(final_graph = (Char200*)new Char200);
(initial_graph = (Char200*)new Char200);
*(rank = new int) = 1;
*(root = new int) = 0;
*(root0 = new int) = 0;
}

}
PROJECT_END_NAMESPACE