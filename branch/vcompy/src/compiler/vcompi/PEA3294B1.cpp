#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int PEA3294B1(TPOData *D)
{
  return (((*(D->FistD))==(*(D->LastD))) || ((*(D->I))==(*(D->LastD))));
}
PROJECT_END_NAMESPACE
