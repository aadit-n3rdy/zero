#ifndef ZERO_ORGANISM_H
#define ZERO_ORGANISM_H

#include <eaux.h>

struct zero_organism {
	struct eaux_perceptron brain;
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
	float food;
	float energy;
	float movement_speed;	// PIXELS/frame
	float angular_movement_speed; // RAD/frame
	float reach;
	float fov; // RAD
	float rov; // RAD
	float angle; // RAD, with horizontal
};

/* INP PARAMS:
 * energy
 * closest seen food angle (rel to self direction)
 * closest seen food distance
 */

/* OUT PARAMS:
 * fwd/bk
 * rotate left/right
 */

int zero_organism_init(struct zero_organism *organism,
		float pos_x, 
		float pos_y);

int zero_organism_update(struct zero_organism *organism,
		struct zero_world *world);

int zero_organism_free(struct zero_organism *organism);

#endif
