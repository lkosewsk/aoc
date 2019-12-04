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
