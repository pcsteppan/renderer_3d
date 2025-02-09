#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"

bool is_running;

float fov_factor = 128 * 6;
vec3_t camera_pos = { 0, 0, -5 };

int prev_frame_time = 0;


void setup(void) {
	frame_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
	if (!frame_buffer) {
		fprintf(stderr, "Error allocating frame buffer.\n");
	}

	texture = SDL_CreateTexture(
		renderer, 
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		window_width,
		window_height
	);
}

float lerp_f(float a, float origin_start, float origin_end, float dest_start, float dest_end) {
	float origin_span = origin_end - origin_start;
	float x = (a - origin_start) / origin_span;
	float destination_span = dest_end - dest_start;
	float y = x * destination_span + dest_start;
	return y;
}

// interpolates a vec2 from a given range to a dest range
vec2_t lerp_vec2_t(vec2_t a, vec2_t origin_start, vec2_t origin_end, vec2_t dest_start, vec2_t dest_end) {
	vec2_t result = {
		lerp_f(a.x, origin_start.x, origin_end.x, dest_start.x, dest_end.x),
		lerp_f(a.y, origin_start.y, origin_end.y, dest_start.y, dest_end.y)
	};

	return result;
}

void process_input(void) {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				is_running = false;
			}
			break;
	}
}

float lose_precision_f(float f, int precision) {
	int s = (int) pow(10, precision);
	return ((float)(int)(f * s)) / s;
}

vec3_t lose_precision(vec3_t v, int precision) {
	return (vec3_t) {
		lose_precision_f(v.x, precision),
		lose_precision_f(v.y, precision),
		lose_precision_f(v.z, precision)
	};
}

// rotate, scale, translate
void transform_points() {
	// for (int i = 0; i < N_POINTS; i++) {
	// 	cube_points[i] = 
	// 		vec3_rotate_z
	// 		(
	// 			vec3_rotate_y
	// 			(
	// 				vec3_rotate_x(cube_points[i], cube_rotation.x),
	// 				cube_rotation.y
	// 			), 
	// 			cube_rotation.z
	// 		);
	// 	// cube_points[i] = lose_precision(cube_points[i], 3);
	// }
}

void project_points() {
	// for (int i = 0; i < N_POINTS; i++)
	// {
	// 	vec3_t pt = cube_points[i];

	// 	pt.z -= camera_pos.z;

	// 	vec2_t pt_2d = { pt.x * fov_factor / pt.z, pt.y * fov_factor / pt.z };

	// 	pt_2d.x += window_width / 2;
	// 	pt_2d.y += window_height / 2;

	// 	vec2_t projected_point = { pt_2d.x, pt_2d.y };
	// 	projected_points[i] = projected_point;
	// }
}

void update(void) {
	transform_points();
	project_points();
}

void draw_projected_points() {
	// for (int i = 0; i < N_POINTS; i++) {
	// 	vec2_t pt = projected_points[i];
	// 	draw_rect((int)pt.x, (int)pt.y, 4, 4, 0xFFEFFFFF);
	// }
}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	clear_frame_buffer(0xFF000000);
	draw_grid(window_width / 20, 0xFF222222, 0xFF000000);
	draw_projected_points();
	render_frame_buffer();

	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		int elapsed = (SDL_GetTicks() - prev_frame_time);
		int time_till_next = FRAME_TARGET_TIME - elapsed;
		if (time_till_next > 0) {
			SDL_Delay(time_till_next);
		}
		
		process_input();
		update();
		render();
	}

	cleanup();

	return 0;
}
