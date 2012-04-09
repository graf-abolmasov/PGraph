#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int ADF8D7A12(TPOData *D)
{
  (*((*(D->GPcomp))+(*(D->root)))).F=1;
  return 1;
}
PROJECT_END_NAMESPACE
