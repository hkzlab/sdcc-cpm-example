#include "cgol.h"

#include <string.h>

static uint8_t gol_grids[2][GRID_WIDTH * GRID_HEIGHT];
static uint8_t gol_idx;

uint8_t *cgol_getGrid(void) {
	return (gol_grids[gol_idx]);
}

void cgol_init(void) {
	memset(gol_grids[0], 0, GRID_WIDTH * GRID_HEIGHT);
	memset(gol_grids[1], 0, GRID_WIDTH * GRID_HEIGHT);

	gol_idx = 0;

	gol_grids[0][15 + (5 * GRID_WIDTH)] = 1;
	gol_grids[0][17 + (6 * GRID_WIDTH)] = 1;
	gol_grids[0][14 + (7 * GRID_WIDTH)] = 1;
	gol_grids[0][15 + (7 * GRID_WIDTH)] = 1;
	gol_grids[0][18 + (7 * GRID_WIDTH)] = 1;
	gol_grids[0][19 + (7 * GRID_WIDTH)] = 1;
	gol_grids[0][20 + (7 * GRID_WIDTH)] = 1;
}

void cgol_step(void) {
	uint8_t *cur_grid = gol_grids[gol_idx];
	uint8_t *dst_grid = gol_grids[(gol_idx + 1) % 2];
	
	uint8_t x, y, neighb;
	uint8_t lx, ty, rx, by;

	for (x = 0; x < GRID_WIDTH; x++) {
		for (y = 0; y < GRID_HEIGHT; y++) {
			lx = (x == 0) ? (GRID_WIDTH - 1) : (x - 1);
			rx = (x == (GRID_WIDTH - 1)) ? 0 : (x + 1);
			ty = (y == 0) ? (GRID_HEIGHT - 1) : (y - 1);
			by = (y == (GRID_HEIGHT - 1)) ? 0 : (y + 1);

			neighb = 0;
			neighb += cur_grid[lx + (y * GRID_WIDTH)];
			neighb += cur_grid[lx + (ty * GRID_WIDTH)];
			neighb += cur_grid[x + (ty * GRID_WIDTH)];
			neighb += cur_grid[rx + (ty * GRID_WIDTH)];
			neighb += cur_grid[rx + (y * GRID_WIDTH)];
			neighb += cur_grid[rx + (by * GRID_WIDTH)];
			neighb += cur_grid[x + (by * GRID_WIDTH)];
			neighb += cur_grid[lx + (by * GRID_WIDTH)];

			if (!cur_grid[x + (y * GRID_WIDTH)] && (neighb == 3))
				dst_grid[x + (y * GRID_WIDTH)] = 1;
			else if (cur_grid[x + (y * GRID_WIDTH)] && ((neighb == 3) || (neighb == 2)))
				dst_grid[x + (y * GRID_WIDTH)] = 1;
			else
				dst_grid[x + (y * GRID_WIDTH)] = 0;
		}
	}

	gol_idx = (gol_idx + 1) % 2;
}
