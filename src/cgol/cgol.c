#include "cgol.h"

#include <string.h>

static uint8_t gol_grids[2][GRID_WIDTH * GRID_HEIGHT];
static uint8_t gol_idx;

static uint8_t precalc_pos_grids[4][GRID_WIDTH * GRID_HEIGHT];
static uint8_t precalc_neighb_grids[2][GRID_WIDTH * GRID_HEIGHT];

uint8_t *cgol_getGrid(void) {
	return (gol_grids[gol_idx]);
}

void cgol_init(void) {
	uint8_t x, y;
	uint8_t lx, ty, rx, by;
	uint16_t cell_addr;

	// Clean the game grids
	memset(gol_grids[0], 0, GRID_WIDTH * GRID_HEIGHT);
	memset(gol_grids[1], 0, GRID_WIDTH * GRID_HEIGHT);

	// Clean the neighbour precalc grids
	memset(precalc_neighb_grids[0], 0, GRID_WIDTH * GRID_HEIGHT);
	memset(precalc_neighb_grids[1], 0, GRID_WIDTH * GRID_HEIGHT);

	// Prepare the precalculated position grids
	for (x = 0; x < GRID_WIDTH; x++) {
		for (y = 0; y < GRID_HEIGHT; y++) {
			cell_addr = x + (y * GRID_WIDTH);

			precalc_pos_grids[0][cell_addr] = (x == 0) ? (GRID_WIDTH - 1) : (x - 1); // lx
			precalc_pos_grids[1][cell_addr] = (x == (GRID_WIDTH - 1)) ? 0 : (x + 1); // rx
			precalc_pos_grids[2][cell_addr] = (y == 0) ? (GRID_HEIGHT - 1) : (y - 1); // ty
			precalc_pos_grids[3][cell_addr] = (y == (GRID_HEIGHT - 1)) ? 0 : (y + 1); // by
		}
	}

	gol_idx = 0;

	gol_grids[gol_idx][15 + (5 * GRID_WIDTH)] = 1;
	gol_grids[gol_idx][17 + (6 * GRID_WIDTH)] = 1;
	gol_grids[gol_idx][14 + (7 * GRID_WIDTH)] = 1;
	gol_grids[gol_idx][15 + (7 * GRID_WIDTH)] = 1;
	gol_grids[gol_idx][18 + (7 * GRID_WIDTH)] = 1;
	gol_grids[gol_idx][19 + (7 * GRID_WIDTH)] = 1;
	gol_grids[gol_idx][20 + (7 * GRID_WIDTH)] = 1;

	// Prepare the initial neighbour precalc grids
	for (x = 0; x < GRID_WIDTH; x++) {
		for (y = 0; y < GRID_HEIGHT; y++) {
			cell_addr = x + (y * GRID_WIDTH);

			lx = precalc_pos_grids[0][cell_addr];
			rx = precalc_pos_grids[1][cell_addr];
			ty = precalc_pos_grids[2][cell_addr];
			by = precalc_pos_grids[3][cell_addr];

			if (gol_grids[gol_idx][cell_addr]) {
				precalc_neighb_grids[gol_idx][lx + (y * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][lx + (ty * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][x + (ty * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][rx + (ty * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][rx + (y * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][rx + (by * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][x + (by * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx][lx + (by * GRID_WIDTH)]++;
			}
		}
	}
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

			lx = precalc_pos_grids[0][cell_addr];
			rx = precalc_pos_grids[1][cell_addr];
			ty = precalc_pos_grids[2][cell_addr];
			by = precalc_pos_grids[3][cell_addr];

			// Use the precalculated neighbour value
			neighb = precalc_neighb_grids[gol_idx][cell_addr];
			precalc_neighb_grids[gol_idx][cell_addr] = 0;

			if ((neighb == 3) || (cur_grid[cell_addr] && (neighb == 2))) {
				dst_grid[cell_addr] = 1;

				// Calculate the neighbour for next step
				precalc_neighb_grids[gol_idx ^ 1][lx + (y * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][lx + (ty * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][x + (ty * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][rx + (ty * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][rx + (y * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][rx + (by * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][x + (by * GRID_WIDTH)]++;
				precalc_neighb_grids[gol_idx ^ 1][lx + (by * GRID_WIDTH)]++;
			} else {
				dst_grid[cell_addr] = 0;
			}
		}
	}

	gol_idx ^= 1;
}
