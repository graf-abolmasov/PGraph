#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int AFA2E8F65(TPOData *D)
{
    int i;
    for(i=0;i<(*(D->MaxLT));i++) {
        (*((*(D->GPcomp))+i)).F=0;
        (*((*(D->GPcomp))+i)).rankT=-1;
    }
    for(i=0;i<(*(D->MaxGf));i++)
        (*((*(D->GPgraf))+i)).F=0;
    (*(D->Faza))=1;
    return 1;
}
PROJECT_END_NAMESPACE
