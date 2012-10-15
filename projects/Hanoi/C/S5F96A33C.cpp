#include "utype.h"
PROJECT_BEGIN_NAMESPACE

extern "C" int printf_bar(TBar *bar, TString *name);
int S5F96A33C(TBar *bar, TString *name)
{
	return printf_bar(bar, name);
}
PROJECT_END_NAMESPACE
