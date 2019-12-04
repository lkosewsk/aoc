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

int sum_3_row(int x_offset, int y_offset) {
	int i, sum = 0;
	for (i = x_offset; i < x_offset + 3; i++)
		sum += grid[i][y_offset];
	return sum;
}

void get_greatest_submatrix(int *big_x, int *big_y, int *mysum)
{
	int i, j;
	int x = 0, y = 0, sum = 0;
	for (i = 0; i < 300 - 2; i++) {
		int prev_rowsum, prev_prev_rowsum;
		for (j = 0; j < 300 - 2; j++) {
			int rowsum = sum_3_row(i, j);
			if (j == 0)
				prev_prev_rowsum = rowsum;
			else if (j == 1)
				prev_rowsum = rowsum;
			else {
				int tot = rowsum + prev_rowsum + prev_prev_rowsum;
				if (tot > sum) {
					sum = tot;
					x = i;
					y = j-2;
				}
				prev_prev_rowsum = prev_rowsum;
				prev_rowsum = rowsum;
			}
		}
	}
	*big_x = x + 1;
	*big_y = y + 1;
	*mysum = sum;
}

int main(int argc, char *argv[])
{
	int great_x, great_y, sum;
	compute_power_levels();
	get_greatest_submatrix(&great_x, &great_y, &sum);
	printf("big x = %d, big y = %d w/sum = %d\n", great_x, great_y, sum);
	//show_grid(19, 59, 5);
	return 0;
}
