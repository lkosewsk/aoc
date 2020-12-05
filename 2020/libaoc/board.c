#include "board.h"
#include "aocinput.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <stdio.h>

struct _board_t {
	char *board_arr;
	int size;
	int rows;
	int cols;
};

board_t *board_create()
{
	char line[1024]; // arbitrary
	board_t *r = (board_t *)malloc(sizeof(board_t));
	r->board_arr = 0;
	r->size = r->rows = r->cols = 0;
	int linelen, numlines = aocinput_count_lines("input");
	input_t *input = aocinput_setup("input");

	// FIXME:  Redo as bits instead of chars?
	while ((linelen = aocinput_getline(input, line))) {
		if (!r->board_arr) {
			// need to set up the array.  We do this by taking numlines times
			// the (non-trailing-newline-included) row size of the input
			r->board_arr = (char *)malloc(numlines * (linelen - 1) + 1);
			r->cols = linelen - 1;
		}
		line[--linelen] = 0;  // delete trailing newline
		strcpy(r->board_arr + r->size, line);
		r->size += linelen;
		r->rows += 1;
	}
	//printf("r->size = %d, numlines = %d, linelen = %d\n", r->size, numlines, linelen);
	assert(r->size == r->rows * r->cols);

	aocinput_teardown(input);
	return r;
}

void board_destroy(board_t *board)
{
	free(board->board_arr);
	free(board);
}

int board_get_at_pos(board_t *board, int col, int row)
{
	assert(row <= board->rows - 1);  // 0-indexed params
	col = col % board->cols;
	return board->board_arr[board->cols * row + col] == '#';
}

int board_get_rows(board_t *board)
{
	return board->rows;
}
