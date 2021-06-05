#include <mosaic.h>
#include "organism.h"
#include "world.h"
#include "../double_list.h"
#include <eaux.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define BRAIN_INPUT_COUNT 5
#define BRAIN_OUTPUT_COUNT 4

#define REPRODUCE_ENERGY_THRESHOLD 30

const unsigned int brain_dimensions[] = {BRAIN_INPUT_COUNT, 10, 10, BRAIN_OUTPUT_COUNT};
const unsigned int brain_layer_count = 4;

float adjust_weights(float inp) {
	float ret = inp;
	if (rand() % 100 < 10) {
		ret += 2.0f * (rand()%100 - 50)/50.0f;
	}
	return ret;
}

struct zero_organism_dbl {
	double_list record;
	struct zero_organism organism;
};

struct zero_food {
	float value;
	float pos_x;
	float pos_y;
};

struct zero_food_dbl {
	double_list record;
	struct zero_food food;
};

int main() {

	srand(time(NULL));

	struct zero_organism_dbl organisms;
	struct zero_food_dbl food;

	double_list_init(&organisms.record);
	double_list_init(&food.record);

	printf("Hello world\n");
	
	while (1) {
		struct zero_organism_dbl *org_ptr = &organisms;
		struct zero_food_dbl *food_ptr = &food;
		float fov;
		float rov_sq;
		float angle;
		float rel_x;
		float rel_y;
		float dev;
		float dist_sq;
		float input_params[BRAIN_INPUT_COUNT];
		bzero(input_params, sizeof(input_params));
		float output_params[BRAIN_OUTPUT_COUNT];
		bzero(output_params, sizeof(output_params));
		float vel_angle;
		float reach_sq;
		char del_cur = 0;
		while (org_ptr != NULL) {
			food_ptr = &food;
			fov = org_ptr->organism.fov/2.0f;
			rov_sq = org_ptr->organism.rov * org_ptr->organism.rov;
			angle = org_ptr->organism.angle;
			reach_sq = org_ptr->organism.reach * org_ptr->organism.reach;
			vel_angle = atan2f(org_ptr->organism.vel_y,
					org_ptr->organism.vel_x) - angle;
			input_params[0] = org_ptr->organism.energy;
			input_params[1] = org_ptr->organism.vel_x * cosf(vel_angle) +
				org_ptr->organism.vel_y * sinf(vel_angle);
			input_params[2] = org_ptr->organism.vel_x * sinf(vel_angle) +
				org_ptr->organism.vel_y * cosf(vel_angle);
			input_params[3] = 0;
			input_params[4] = 0;
			del_cur = 0;
			while (food_ptr != NULL) {
				rel_x = food_ptr->food.pos_x - org_ptr->organism.pos_x;
				rel_y = food_ptr->food.pos_y - org_ptr->organism.pos_y;
				dist_sq = rel_x * rel_x + rel_y * rel_y; 
				if (dist_sq <= reach_sq) {
					org_ptr->organism.energy += food_ptr->food.value;
					input_params[0] += food_ptr->food.value;
					double_list_remove((double_list *)food_ptr);
				} else if (dist_sq <= rov_sq && 
						(dist_sq < input_params[4] * input_params[4] 
						 || input_params[4] == 0)) {
					dev = atan2f(rel_y, rel_x) - angle;
					if (fabs(dev) < fov) {
						input_params[3] = dev;
						input_params[4] = sqrtf(dist_sq);
					}
				}
				if (del_cur) {
					struct zero_food_dbl *food_ptr_temp = (struct zero_food_dbl*)(food_ptr_temp->record.next);
					free(food_ptr);
					food_ptr = food_ptr_temp;
				} else {
					food_ptr = 
						(struct zero_food_dbl*)(food_ptr->record.next);
				}
			}
			eaux_perceptron_calc(&org_ptr->organism.brain, input_params, output_params);
			// TODO:
			// Check if can reproduce, if wants to reproduce
			// 	add new kid using eaux_perceptron_cpy for brain,
			// 	and mutate
			// Deduct energy based on vel
			// Check if dead, and if so remove


			/* TODO: MOVE THIS CODE TO MUTATION PART
			unsigned int i = 0;
			while (i < brain_layer_count-1) {
				mosaic_matrix_apply(&org_ptr->organism.brain.weights[i],
						adjust_weights);
				i++;
			}*/
		}
	}
	return 0;
}
