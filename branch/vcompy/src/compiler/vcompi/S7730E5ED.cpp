#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int BcodeTr(Char200 *Cod,char *WR,char *Tip,int *Nurov,int *Kurov);
int S7730E5ED(Char200 *Cod,char *WR,char *Tip,int *Nurov,int *Kurov)
{
	return BcodeTr(Cod, WR, Tip, Nurov, Kurov);
}
PROJECT_END_NAMESPACE
