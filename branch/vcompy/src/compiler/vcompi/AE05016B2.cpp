#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int AE05016B2(TPOData *D)
{
  if((*((*(D->GPcomp))+(*(D->FromTop)))).Faz > 0)
      (*(D->Faza))=(*((*(D->GPcomp))+(*(D->FromTop)))).Faz;
  return 1;
}
PROJECT_END_NAMESPACE
