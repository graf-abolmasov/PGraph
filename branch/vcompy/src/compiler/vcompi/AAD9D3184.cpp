#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int AAD9D3184(TPOData *D)
{
  (*(D->FistD))=(*((*(D->GPcomp))+(*(D->FromTop)))).FirstDef; (*(D->LastD))=(*((*(D->GPcomp))+(*(D->FromTop)))).LastDef; strncpy((*(D->CodPU)),(*((*(D->GPcomp))+(*(D->FromTop)))).CodeTr,199);
  return 1;
}
PROJECT_END_NAMESPACE
