#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"

bool is_running;

#define N_POINTS (9 * 9 * 9)
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

float fov_factor = 128 * 4;
vec3_t camera_pos = { 0, 0, -5 };

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
	
	int i = 0;
	float delta = 0.25;
	for (float x = -1; x <= 1; x += delta) {
		for (float y = -1; y <= 1; y += delta) {
			for (float z = -1; z <= 1; z += delta) {
				printf("%d\n", i);
				cube_points[i] = (vec3_t){ x, y, z };
				i += 1;
			}
		}
	}
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
	// TODO
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

void update(void) {
	for (int i = 0; i < N_POINTS; i++)
	{
		vec3_t pt = cube_points[i];

		pt.z -= camera_pos.z;

		vec2_t pt_2d = { pt.x * fov_factor / pt.z, pt.y * fov_factor / pt.z };

		pt_2d.x += window_width / 2;
		pt_2d.y += window_height / 2;

		vec2_t projected_point = { pt_2d.x, pt_2d.y };
		// vec2_t projected_point = lerp_vec2_t(
		// 	pt_2d, 
		// 	(vec2_t){ -1, -1 }, 
		// 	(vec2_t){ 1, 1, }, 
		// 	(vec2_t){ 400, 200 }, 
		// 	(vec2_t){ 600, 400 });
		projected_points[i] = projected_point;
	}
}

void draw_projected_points() {
	for (int i = 0; i < N_POINTS; i++) {
		vec2_t pt = projected_points[i];
		draw_rect((int)pt.x, (int)pt.y, 2, 2, 0xFFFF0000);
	}
}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	clear_frame_buffer(0xFF000000);
	draw_grid(window_width / 20, 0xFFBBBBBB, 0xFF0000FF);
	draw_rect(300, 100, 100, 100, 0xFFFF0000);
	draw_pixel(600, 820, 0xFF00FF00);
	draw_projected_points();
	render_frame_buffer();

	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	cleanup();

	return 0;
}
