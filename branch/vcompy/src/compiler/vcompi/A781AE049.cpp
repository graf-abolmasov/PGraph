#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int A781AE049(TPOData *D)
{
  strncpy((*(D->CodPU)),(*((*(D->GPcomp))+(*(D->FromTop)))).CodeTr,199);
  return 1;
}
PROJECT_END_NAMESPACE
