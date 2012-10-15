#include "utype.h"

int move2(TBar *from, TBar *to, int *from_top, int *to_top) {
	int from_idx = RINGS_MAX-1;
	int to_idx = RINGS_MAX-1;

	while ((*from)[from_idx] == 0 && from_idx >= 0)
		from_idx--;

	while ((*to)[to_idx] == 0 && to_idx >= 0)
		to_idx--;	

	if (from_idx == -1) {
		return -1;
	}

	if (to_idx == RINGS_MAX-1) {
		return -2;
	}

	if (to_idx > -1 && ((*from)[from_idx] < (*to)[to_idx])) {
		return -3;
	}

	(*to)[to_idx+1] = (*from)[from_idx];
	(*from)[from_idx] = 0;
	if (from_idx == 0)
		(*from_top) = 0;
	else
		(*from_top) = (*from)[from_idx-1];

	if (to_idx == RINGS_MAX-1)
		(*to_top) = (*to)[RINGS_MAX-1];
	else
		(*to_top) = (*to)[to_idx+1];

	return 0;
}
