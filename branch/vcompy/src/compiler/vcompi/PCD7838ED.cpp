#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int PCD7838ED(TPOData *D)
{
  return ((*((*(D->GPcomp))+(*(D->NTop)))).F==0);
}
PROJECT_END_NAMESPACE
