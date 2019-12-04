#include <stdio.h>

int serial = 9110;
int grid[300][300];

void compute_power_levels()
{
	int i, j;

	for (i = 0; i < 300; i++) { // x coord
		for (j = 0; j < 300; j++) {  // y coord
			//printf("for i, j = (%d, %d):\n", i, j);
			int rack_id = (i + 1) + 10;
			//printf(  "rack_id = %d\n", rack_id);
			int power_level = rack_id * (j + 1);
			//printf("  rack_id * y = %d\n", power_level);
			power_level += serial;
			//printf("  +serial = %d\n", power_level);
			power_level *= rack_id;
			//printf("  *rack_id= %d\n", power_level);
			power_level = (power_level % 1000) / 100;
			//printf("  hundreds_col= %d\n", power_level);
			power_level -= 5;
			//printf("  -5= %d\n", power_level);
			grid[i][j] = power_level;
		}
	}
}

void show_grid(int x, int y, int square_size)
{
	int i, j;
	for (i = y; i < y + square_size; i++) {
		for (j = x; j < x + square_size; j++) {
			printf("%d ", grid[j][i]);
		}
		printf("\n");
	}
}

int sum_n_row(int x_offset, int y_offset, int n) {
	int i, sum = 0;
	for (i = x_offset; i < x_offset + n; i++)
		sum += grid[i][y_offset];
	return sum;
}

typedef struct _movingbuf {
	int *start, *cur;
	int buf[300];
} movingbuf;
movingbuf sumbuf;

int sum_whole_buf()
{
	int *cnt, tot = 0;
	for (cnt = sumbuf.start; cnt < sumbuf.cur; cnt++) {
		tot += *cnt;
	}
	return tot;
}

void get_greatest_submatrix(int size, int *big_x, int *big_y, int *mysum)
{
	int i, j;
	int x = 0, y = 0, sum = 0;
	int never_used = 1;
	int running_sum;

	for (i = 0; i < 300 - (size - 1); i++) {
		sumbuf.start = sumbuf.cur = sumbuf.buf;
		running_sum = 0;
		for (j = 0; j < 300 /*- (size - 1)*/; j++) {
			int rowsum = sum_n_row(i, j, size);
			*sumbuf.cur++ = rowsum;
			running_sum += rowsum;
			if (j >= size - 1) {
				//int tot = sum_whole_buf();
				int tot = running_sum;
				if (never_used || tot > sum) {
					sum = tot;
					x = i;
					y = j-(size - 1);
					never_used = 0;
				}
				running_sum -= *sumbuf.start++;
			}
		}
	}
	*big_x = x + 1;
	*big_y = y + 1;
	*mysum = sum;
}

void output_matrix()
{
	show_grid(0, 0, 300);
}

int main(int argc, char *argv[])
{
	int i, greatest_x, greatest_y, bigsum, matrix_size = 0;
	int great_x, great_y, sum;
	compute_power_levels();
	//output_matrix();
	//return 0;
	for (i = 1; i <= 300; i++) {
		get_greatest_submatrix(i, &great_x, &great_y, &sum);
		if (!matrix_size || sum > bigsum) {
			greatest_x = great_x;
			greatest_y = great_y;
			bigsum = sum;
			matrix_size = i;
		}
	}	
	printf("big x = %d, big y = %d w/sum = %d and size=%d\n", greatest_x, greatest_y, bigsum, matrix_size);
	//show_grid(19, 59, 5);
	return 0;
}
