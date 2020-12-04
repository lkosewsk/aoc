#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define debug(x, y)
//#define debug(x, y) printf(x, y)

int validate_pos(char *haystack, int haylen, char needle, int pos)
{
	if (pos > haylen)
		return 0;
	// remember we swallowed the leading space when we read the input, so we
	// don't need to compensate by doing pos - 1 here
	return (haystack[pos] == needle);
}

int only_one(char *haystack, char needle, int pos1, int pos2)
{
	int haylen = strlen(haystack);
	int valid_cnt = validate_pos(haystack, haylen, needle, pos1);
	valid_cnt += validate_pos(haystack, haylen, needle, pos2);
	return valid_cnt == 1;
}

int main()
{
	int r, numgood = 0, numtotal = 0;;
	char buf[1024]; //arbitrary
	input_t *file = aocinput_setup("input");

	while (1) {
		int start, end;
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
		if (only_one(buf, c, start, end))
			numgood++;

		numtotal++;
		//break;
	}

	printf("%d valid passwords out of %d\n", numgood, numtotal);

	aocinput_teardown(file);
}
