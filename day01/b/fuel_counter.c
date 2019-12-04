#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>

int calculate_fuel(int weight)
{
	int a = weight / 3 - 2;
	if (a <= 0)
		return 0;
	return a + calculate_fuel(a);
}

int main()
{
	char linebuf[2048];
	input_t *pfinfo;

	pfinfo = setup_input("../input.txt");

	int line_len;
	int total = 0;
	while (line_len = get_a_line(pfinfo, linebuf)) {
		int a = calculate_fuel(atoi(linebuf));
		total += a;
	}

	printf("%d\n", total);

	teardown_input(pfinfo);

	return 0;
}
