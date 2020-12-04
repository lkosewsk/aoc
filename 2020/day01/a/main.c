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
	int *i, *read_nums = malloc(sizeof(int) * num_nums);

	input_t *f = aocinput_setup("input");

	// Read in all the numbers
	while ((charsread = aocinput_getline(f, linebuf))) {
		read_nums[num_read_nums++] = atoi(linebuf);
	}
	assert(num_nums == num_read_nums);

	// sort them
	qsort(read_nums, num_nums, sizeof(int), intcmpfunc);

	// iterate through each one, binary searching the rest of the array to
	// see if the complementary number (that sums with me to 2020) is there.
	for (i = read_nums; i < read_nums + num_nums; ++i) {
		int complement = 2020 - *i;
		void *eureka = bsearch(&complement, i, num_nums, sizeof(int),
					intcmpfunc);
		if (eureka) {
			printf("%d * %d = %d\n", *i, complement,
				*i * complement);
			break;
		}

	}

	aocinput_teardown(f);
	return 0;
}
