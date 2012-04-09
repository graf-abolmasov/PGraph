#include "utype.h"
#include <stdio.h>
#include <string.h>
PROJECT_BEGIN_NAMESPACE
extern int ReadTr(Char200 *Cod,char *Tip,int *Nurov,int *Kurov);
int S19E6D097(Char200 *Cod,char *Tip,int *Nurov,int *Kurov)
{
	return ReadTr(Cod, Tip, Nurov, Kurov);
}
PROJECT_END_NAMESPACE
