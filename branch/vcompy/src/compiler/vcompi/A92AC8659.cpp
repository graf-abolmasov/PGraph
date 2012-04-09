#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int A92AC8659(TPOData *D)
{
    (*((*(D->GPgraf))+(*(D->I)))).F=1;
    strncpy((*((*(D->GPgraf))+(*(D->I)))).CodeTr,(*(D->CodPU)),199);
    return 1;
}
PROJECT_END_NAMESPACE
