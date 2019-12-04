#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
	char linebuf[2048];
	input_t *pfinfo;

	pfinfo = aocinput_setup("../input.txt");

	int line_len;
	int total = 0;
	while (line_len = aocinput_getline(pfinfo, linebuf)) {
		int n = atoi(linebuf);
		int a = n / 3 - 2;
		total += a;
	}

	printf("%d\n", total);

	aocinput_teardown(pfinfo);

	return 0;
}
