#include "cgol.h"

#include <string.h>

static uint8_t gol_grids[2][GRID_WIDTH * GRID_HEIGHT];
static uint8_t gol_idx;

static uint8_t precalc_grids[4][GRID_WIDTH * GRID_HEIGHT];

uint8_t *cgol_getGrid(void) {
	return (gol_grids[gol_idx]);
}

void cgol_init(void) {
	uint8_t x, y;
	uint16_t cell_addr;

	// Clean the game grids
	memset(gol_grids[0], 0, GRID_WIDTH * GRID_HEIGHT);
	memset(gol_grids[1], 0, GRID_WIDTH * GRID_HEIGHT);

	// Prepare the precalculated grids
	for (x = 0; x < GRID_WIDTH; x++) {
		for (y = 0; y < GRID_HEIGHT; y++) {
			cell_addr = x + (y * GRID_WIDTH);

			precalc_grids[0][cell_addr] = (x == 0) ? (GRID_WIDTH - 1) : (x - 1); // lx
			precalc_grids[1][cell_addr] = (x == (GRID_WIDTH - 1)) ? 0 : (x + 1); // rx
			precalc_grids[2][cell_addr] = (y == 0) ? (GRID_HEIGHT - 1) : (y - 1); // ty
			precalc_grids[3][cell_addr] = (y == (GRID_HEIGHT - 1)) ? 0 : (y + 1); // by
		}
	}

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
	uint8_t *dst_grid = gol_grids[gol_idx ^ 1];
	
	uint8_t x, y, neighb;
	uint8_t lx, ty, rx, by;
	uint16_t cell_addr;

	for (x = 0; x < GRID_WIDTH; x++) {
		for (y = 0; y < GRID_HEIGHT; y++) {
			cell_addr = x + (y * GRID_WIDTH);

			lx = precalc_grids[0][cell_addr];
			rx = precalc_grids[1][cell_addr];
			ty = precalc_grids[2][cell_addr];
			by = precalc_grids[3][cell_addr];

			neighb = 0;
			neighb += cur_grid[lx + (y * GRID_WIDTH)];
			neighb += cur_grid[lx + (ty * GRID_WIDTH)];
			neighb += cur_grid[x + (ty * GRID_WIDTH)];
			neighb += cur_grid[rx + (ty * GRID_WIDTH)];
			neighb += cur_grid[rx + (y * GRID_WIDTH)];
			neighb += cur_grid[rx + (by * GRID_WIDTH)];
			neighb += cur_grid[x + (by * GRID_WIDTH)];
			neighb += cur_grid[lx + (by * GRID_WIDTH)];


			if ((neighb == 3) || (cur_grid[cell_addr] && (neighb == 2)))
				dst_grid[cell_addr] = 1;
			else
				dst_grid[cell_addr] = 0;
		}
	}

	gol_idx ^= 1;
}
