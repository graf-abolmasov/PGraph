#include "utype.h"
#include "utype.h"PROJECT_BEGIN_NAMESPACE

extern "C" int move2(TBar *from, TBar *to, int *from_top, int *to_top);
int SA4263A50(TBar *from, TBar *to, int *from_top, int *to_top)
{
	return move2(from, to, from_top, to_top);
}
PROJECT_END_NAMESPACE
