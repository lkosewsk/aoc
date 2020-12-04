#include "wvtest.h"
#include "aocinput.h"

#include "string.h"

WVTEST_MAIN("basic functionality")
{
	char linebuf[512];
	input_t *inp = aocinput_setup("t/basic.txt");

	int l = aocinput_getline(inp, linebuf);
	WVPASSEQ(l, 2);

	aocinput_teardown(inp);
}

WVTEST_MAIN("ensure works even with no final char")
{
	char linebuf[512];
	input_t *inp = aocinput_setup("t/nolf.txt");

	int l = aocinput_getline(inp, linebuf);
	WVPASSEQ(l, strlen(linebuf));
	WVFAILEQ(linebuf[l - 1], '\n');
	WVPASSEQSTR(linebuf, "noeol");

	aocinput_teardown(inp);
}

WVTEST_MAIN("produce counts of what would be read and no crash with null buf")
{
	input_t *inp = aocinput_setup("t/basic.txt");

	int l = aocinput_getline(inp, 0);
	WVPASSEQ(l, 2);
	l = aocinput_getline(inp, 0);
	WVPASSEQ(l, 4);

	aocinput_teardown(inp);
}

WVTEST_MAIN("does count_lines work with lf and no-lf terminated files?")
{
	typedef struct {
		char filename[20];
		int expected_linecount;
	} e;
	e *i;
	e expected_output[] = {
		{
			.filename = "t/basic.txt",
			.expected_linecount = 2,
		},
		{
			.filename = "t/nolf.txt",
			.expected_linecount = 1,
		}
	};
	for (i = expected_output;
	     i < expected_output + sizeof(expected_output) / sizeof(*i);
	     i++) {
		int r = aocinput_count_lines(i->filename);
		WVPASSEQ(r, i->expected_linecount);
	}
}
