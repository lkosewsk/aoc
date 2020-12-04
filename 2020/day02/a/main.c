#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>

#define debug(x, y)
//#define debug(x, y) printf(x, y)

int count_chars(char *haystack, char needle)
{
	char *c;
	int r = 0;
	for (c = haystack; *c; c++) {
		if (*c == needle)
			r++;
	}
	return r;
}

int main()
{
	int r, numgood = 0, numtotal = 0;;
	char buf[1024]; //arbitrary
	input_t *file = aocinput_setup("input");

	while (1) {
		int start, end, num_occurrences;
		char c;
		// litmus test - if empty, we're done, otherwise it's the start
		r = aocinput_get_to_sep(file, buf, '-');
		if (!r)
			break;
		buf[r - 1] = 0;  // clear -
		start = atoi(buf);
		debug("start = %d\n", start);

		// end of range
		r = aocinput_get_to_sep(file, buf, ' ');
		buf[r - 1] = 0;  // clear ' '
		end = atoi(buf);
		debug("end = %d\n", end);

		// magic char
		r = aocinput_get_to_sep(file, buf, ':');
		c = buf[0];
		debug("c = %c\n", c);

		// count number of times c is in buf and report good or bad;
		r = aocinput_getline(file, buf);
		debug("buf = %s\n", buf);
		num_occurrences = count_chars(buf, c);
		if (num_occurrences >= start && num_occurrences <= end)
			numgood++;

		numtotal++;
		//break;
	}

	printf("%d valid passwords out of %d\n", numgood, numtotal);

	aocinput_teardown(file);
}
