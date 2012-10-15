#include "utype.h"

int is_grater(TBar *left, TBar *right) {
	int left_idx = RINGS_MAX-1;
	int right_idx = RINGS_MAX-1;

	while ((*left)[left_idx] == 0 && left_idx >= 0)
		left_idx--;

	while ((*right)[right_idx] == 0 && right_idx >= 0)
		right_idx--;	

	if (left_idx == -1 && right_idx != -1) {
		return 0;
	}

	if (right_idx == -1 && left_idx != -1) {
		return 1;
	}

	if (((*left)[left_idx] > (*right)[right_idx])) {
		return 1;
	}

	return 0;
}
