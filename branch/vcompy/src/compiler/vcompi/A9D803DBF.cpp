#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int A9D803DBF(TPOData *D)
{
    (*(D->root))=(*(D->root0)); (*(D->FromTop))=(*(D->root));
    (*((*(D->GPcomp))+(*(D->FromTop)))).Faz=1;
    //strncpy((*((*(D->GPcomp))+(*(D->FromTop)))).CodeTr,"0.V",199);
    return 1;
}
PROJECT_END_NAMESPACE
