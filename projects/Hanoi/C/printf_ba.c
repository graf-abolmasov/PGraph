#include "utype.h"

int printf_bar(TBar *bar, TString *name) {
	int idx;

	printf("%s = ", (*name));
	for (idx = 0; idx < RINGS_MAX; idx++) {
		printf("%d ", (*bar)[idx]);
	}
	printf("\r\n");

	return 0;
}
