#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int S7AE89170(COMPTOPs *GPcomp, int *MaxLT);
int P43C2377C(TPOData *D)
{
//Параллельный ли граф
    COMPTOPs *_GPcomp = D->GPcomp;
    int *_MaxLT = D->MaxLT;

	return S7AE89170(_GPcomp, _MaxLT);

}
PROJECT_END_NAMESPACE
