#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int A71A7531D(TPOData *D)
{
  strncpy((*(D->CodPU)), (*((*(D->GPgraf))+(*(D->I))-1)).CodeTr,199);
  return 1;
}
PROJECT_END_NAMESPACE
