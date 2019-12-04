#include "wvtest.h"
#include "aocinput.h"

WVTEST_MAIN("basic functionality")
{
	char linebuf[512];
	input_t *inp = setup_input("t/basic.txt");

	int l = get_a_line(inp, linebuf);
	WVPASSEQ(l, 2);

	teardown_input(inp);
}
