#include "mosaic.h"
#include "organism.h"
#include "world.h"
#include "../double_list.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <eaux.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define BRAIN_INPUT_COUNT 5
#define BRAIN_OUTPUT_COUNT 3

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
	struct zero_world world;
	struct zero_organism_dbl organisms;
	struct zero_food_dbl food;
	FILE *fp;

	fp = popen("stty size", "r");
	fscanf(fp, "%u %u", &world.height, &world.width);
	world.grid_size = world.height * world.width;
	fclose(fp);

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("ERROR: could not init sdl\n");
	}
	IMG_Init(IMG_INIT_PNG);

	SDL_Window *win = SDL_CreateWindow("ZERO",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			0);

	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	while (1) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
					return 0;
			};
		}

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);

		SDL_RenderClear(rend);

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
		float output_params[BRAIN_OUTPUT_COUNT];
		float vel_angle;
		float reach_sq;
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
				food_ptr = (struct zero_food_dbl*)(food_ptr->record.next);
				if (food_ptr->record.next->prev != &food_ptr->record) {
					free(food_ptr);
				}
			}
			eaux_perceptron_calc(&org_ptr->organism.brain, input_params, output_params);
			/* TODO: MOVE THIS CODE TO MUTATION PART
			unsigned int i = 0;
			while (i < brain_layer_count-1) {
				mosaic_matrix_apply(&org_ptr->organism.brain.weights[i],
						adjust_weights);
				i++;
			}*/
		}
		// GAME LOOP
		//
		// UPDATE ALL ORGANISMS
		//
		// REMOVE DEAD ONES
		//
		// ADD NEWBORNS
		//
		// DISPLAY
		//
		// RESET RENGER GRID
		//
		SDL_RenderPresent(rend);
	}
	return 0;
}
