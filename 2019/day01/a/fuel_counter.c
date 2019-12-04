#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>

int main()
{
	char linebuf[2048];
	input_t *pfinfo;

	pfinfo = setup_input("../input.txt");

	int line_len;
	int total = 0;
	while (line_len = get_a_line(pfinfo, linebuf)) {
		int n = atoi(linebuf);
		int a = n / 3 - 2;
		total += a;
	}

	printf("%d\n", total);

	teardown_input(pfinfo);

	return 0;
}
