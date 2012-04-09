#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int A8C6F61DD(TPOData *D)
{
    //strncpy((*((*(D->GPcomp))+(*((*(D->GPgraf))+(*(D->I)))).NambTop)).SPName,(*(D->NameGr)),8);
    (*((*(D->GPcomp))+(*((*(D->GPgraf))+(*(D->I)))).NambTop)).back=(*(D->FromTop));
    (*((*(D->GPcomp))+(*((*(D->GPgraf))+(*(D->I)))).NambTop)).rankT=(*(D->rank));
    (*(D->rank))++;
    return 1;
}
PROJECT_END_NAMESPACE
