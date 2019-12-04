#include "aocinput.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int parse_next(int *input, int pos)
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
		return 0;
	default:
		return -1;
	}
	return 1;  // means continue
}

#define COMPUTER_MEM 512
static int orig_input[COMPUTER_MEM];
static int input[COMPUTER_MEM];
static int prog_len = 0;

// return midpoint of low and high, rounding down
int midpoint(int low, int high)
{
	return (low + high) / 2;
}

void run_computer()
{
	for (int j = 0; j < prog_len; j += 4) {
		int r = parse_next(input, j);
		switch(r) {
		case -1:
			printf("ERROR at pos %d\n", j);
			// fall through
		case 0:
			return;
		case 1:
			// fall through
		default:
			break;
		}
	}
}

void find_inputs(int target)
{
	// linearly go through input 1, and vary input 2.  Assume that result
	// is monotonic since we can only + and *
	for (int i1 = 0; i1 <= 99; ++i1) {
		int i2_low = 0;
		int i2_high = 99;
		int last_input = -1;
		printf("Trying %d\n", i1);
		while (1) {
			memcpy(input, orig_input, sizeof(int) * COMPUTER_MEM);
			input[1] = i1;
			if (last_input == midpoint(i2_low, i2_high)) {
				if (last_input == 0 || last_input == 99) {
					// can't reach our desired output
					// because we're too high or too low
					// at boundary value
					break;
				}
				printf("Infinite recursion with %d and %d, midpoint values (%d, %d)\n",
					i1, last_input, i2_low, i2_high);
				break;
			}
			last_input = midpoint(i2_low, i2_high);
			input[2] = last_input;
			printf("- Trying i2 = %d\n", last_input);

			run_computer();

			printf("Output: %d\n", input[0]);
			if (input[0] < target) {
				// output too low, raise input
				i2_low = last_input + 1;
			} else if (input[0] > target) {
				// output too high, lower input
				i2_high = last_input - 1;
			} else {
				// success!
				printf("Result is %d and %d\n", input[1],
				       input[2]);
				printf("Therefore: %d\n", input[1] * 100 + input[2]);
				return;
			}
		}
	}
}

int main()
{
	char linebuf[512];
	int l;

	input_t *f = aocinput_setup("../input.txt");

	while (l = aocinput_get_to_sep(f, linebuf, ',')) {
		input[prog_len++] = atoi(linebuf);
	}

	aocinput_teardown(f);

	memcpy(orig_input, input, sizeof(int) * COMPUTER_MEM);

	find_inputs(19690720);

	return 0;
}
