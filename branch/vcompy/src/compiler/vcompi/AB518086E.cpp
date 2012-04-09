#include "tpodata.h"
PROJECT_BEGIN_NAMESPACE
int AB518086E(TPOData *D)
{
  (*(D->FistD))=(*((*(D->GPcomp))+(*(D->FromTop)))).FirstDef; (*(D->LastD))=(*((*(D->GPcomp))+(*(D->FromTop)))).LastDef; (*(D->ArcType))=(*((*(D->GPgraf))+(*(D->FistD)))).ArcType_;
  return 1;
}
PROJECT_END_NAMESPACE
