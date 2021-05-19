#ifndef ZERO_WORLD_H
#define ZERO_WORLD_H

#include <wchar.h>

unsigned int world_rows = 600;
unsigned int world_cols = 800;

enum zero_grid_datatypes {
	ZERO_GRID_EMPTY,
	ZERO_GRID_ORGANISM,
	ZERO_GRID_FOOD
};

struct zero_world {
	unsigned int width;
	unsigned int height;
	unsigned int grid_size;
	enum zero_grid_datatypes *grid;
	void **grid_data;
	wchar_t *render_grid;
};

int zero_world_init(struct zero_world *world);
int zero_world_update(struct zero_world *world);
int zero_world_free(struct zero_world *world);

#endif
