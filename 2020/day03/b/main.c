#include "board.h"

#include <stdio.h>

typedef struct _slope {
	int right;
	int down;
} slope;

int main()
{
	slope wanted_slopes[] = {
		{ 1, 1 }, { 3, 1 }, { 5, 1 }, { 7, 1 }, { 1, 2 }
	};
	slope *curslope;
	int rows;
	long total_multiple = 1;
	board_t *board = board_create();
	rows = board_get_rows(board);

	for (curslope = wanted_slopes;
	     curslope < wanted_slopes + sizeof(wanted_slopes) / sizeof(slope);
	     curslope++) {
		int col, row, trees = 0;
		for (col = curslope->right, row = curslope->down;
		     row < rows;
		     col += curslope->right, row += curslope->down) {
			int curval = board_get_at_pos(board, col, row);
			trees += curval;
		}
		printf("For %p (%d, %d), trees = %d\n", curslope, curslope->right,
							curslope->down,
							trees);
		total_multiple *= trees;
	}

	printf("Total tree multiple = %ld\n", total_multiple);

	board_destroy(board);
	return 0;
}
