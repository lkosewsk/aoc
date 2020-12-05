#ifndef BOARD_H
#define BOARD_H

typedef struct _board_t board_t;

board_t *board_create();
void board_destroy(board_t *);

// 1 == tree, 0 == free.  col & row 0-indexed
int board_get_at_pos(board_t *, int col, int row);

int board_get_rows(board_t *);

#endif
