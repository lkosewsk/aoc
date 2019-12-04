#include "wvtest.h"
#include "aocinput.h"

WVTEST_MAIN("basic functionality")
{
	char linebuf[512];
	input_t *inp = aocinput_setup("t/basic.txt");

	int l = aocinput_getline(inp, linebuf);
	WVPASSEQ(l, 2);

	aocinput_teardown(inp);
}
