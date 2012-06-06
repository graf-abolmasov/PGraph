#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int AB5D3E698(TPOData *D)
{
  strncpy((*((*(D->GPcomp))+(*(D->FromTop)))).SPName,(*(D->NameGr)),NAMEPR_SIZE);
  return 1;
}
PROJECT_END_NAMESPACE
