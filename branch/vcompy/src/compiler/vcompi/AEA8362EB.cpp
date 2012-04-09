#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int AEA8362EB(TPOData *D)
{
    strncpy((*(D->CodPU)),"0.V",4);
    strncpy((*(D->NameGr)),"Wg",3);
    return 1;
}
PROJECT_END_NAMESPACE
