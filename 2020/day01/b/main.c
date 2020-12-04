#include "aocinput.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

static int intcmpfunc(const void *a, const void *b)
{
	return (*(int *)a - *(int *)b);
}

int main()
{
	char linebuf[20];  // arbitrary
	int charsread = 0, num_read_nums = 0;
	int num_nums = aocinput_count_lines("input");
	int *i, *j, *read_nums = malloc(sizeof(int) * num_nums);

	input_t *f = aocinput_setup("input");

	// Read in all the numbers
	while ((charsread = aocinput_getline(f, linebuf))) {
		read_nums[num_read_nums++] = atoi(linebuf);
	}
	assert(num_nums == num_read_nums);

	// sort them
	qsort(read_nums, num_nums, sizeof(int), intcmpfunc);

	// iterate through each one, and the rest of the array trying to find
	// that elusive third number that sums them all to 2020.  Short circuit
	// if we find the two added are greater than 2020.
	for (i = read_nums; i < read_nums + num_nums; ++i) {
		for (j = i + 1; j < read_nums + num_nums; ++j) {
			int missing_third = 2020 - *i - *j;
			if (missing_third <= 0)
				break;  // back to outer loop
			void *eureka = bsearch(&missing_third,
						j + 1,
						num_nums - (j - read_nums) - 1,
					       	sizeof(int),
						intcmpfunc);
			if (eureka) {
				printf("%d * %d * %d = %d\n", *i, *j,
						missing_third,
						*i * *j * missing_third);
				break;
			}
		}
	}

	aocinput_teardown(f);
	return 0;
}
