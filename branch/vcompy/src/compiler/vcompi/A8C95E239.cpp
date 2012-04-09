#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
extern int print_struct(COMPTOPs *ListTop,DEFGRAFs *LstGraf, int *MaxLT, int *MaxGf);
int A8C95E239(TPOData *D)
{
    print_struct(D->GPcomp, D->GPgraf, D->MaxLT, D->MaxGf);
    return 1;
}
PROJECT_END_NAMESPACE
