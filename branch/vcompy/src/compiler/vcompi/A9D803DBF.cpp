#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int A9D803DBF(TPOData *D)
{
    (*(D->root))=(*(D->root0));
    (*(D->FromTop))=(*(D->root));
    (*((*(D->GPcomp))+(*(D->FromTop)))).Faz=1;
    return 1;
}
PROJECT_END_NAMESPACE
