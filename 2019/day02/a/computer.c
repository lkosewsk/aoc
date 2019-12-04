#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>

static int parse_next(int *input, int length, int pos)
{
	int regi1, regi2, rego;
	int opcode = input[pos];

	regi1 = input[pos + 1];
	regi2 = input[pos + 2];
	rego = input[pos + 3];
	switch (opcode) {
	case 1:
		input[rego] = input[regi1] + input[regi2];
		break;
	case 2:
		input[rego] = input[regi1] * input[regi2];
		break;
	case 99:
		return -1;
	default:
		return -2;
	}
	return 0;
}

int main()
{
	char linebuf[512];
	int input[512];
	int l, i = 0;

	input_t *f = aocinput_setup("../input.txt");

	while (l = aocinput_get_to_sep(f, linebuf, ',')) {
		input[i++] = atoi(linebuf);
	}

	// per instructions
	input[1] = 12;
	input[2] = 2;
	for (int j = 0; j < i; j += 4) {
		int r = parse_next(input, i, j);
		if (r == -2) {
			printf("ERROR at pos %d\n", j);
			break;
		}
		if (r == -1) {
			printf("%d\n", input[0]);
			break;
		}
	}

	aocinput_teardown(f);
	return 0;
}
