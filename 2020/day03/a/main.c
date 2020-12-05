#include "board.h"

#include <stdio.h>

int main()
{
	int col, row, rows, trees = 0;
	board_t *board = board_create();
	rows = board_get_rows(board);

	for (col = 3, row = 1; row < rows; col +=3, row += 1) {
		int curval = board_get_at_pos(board, col, row);
		//printf("At %d, %d = %d\n", col, row, curval);
		trees += curval;
	}

	printf("Num trees encountered = %d\n", trees);

	board_destroy(board);
	return 0;
}
