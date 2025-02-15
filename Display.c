#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Array.h"

SDL_Window* window;
SDL_Renderer* renderer;

int window_width = 800;
int window_height = 600;

uint32_t* frame_buffer;
SDL_Texture* texture;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Query SDL for fullscreen max width + height
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	window_width = display_mode.w;
	window_height = display_mode.h;

	int x, y;
	x = y = SDL_WINDOWPOS_CENTERED;
	// create SDL window
	window = SDL_CreateWindow(NULL, x, y, window_width, window_height, SDL_WINDOW_BORDERLESS);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void draw_grid(int size, uint32_t grid_color, uint32_t background_color) {
	for (int row = 0; row < window_height; row++) {
		for (int col = 0; col < window_width; col++) {
			if (row % size == 0 || col % size == 0) {
				frame_buffer[row * window_width + col] = grid_color;
			} else {
				frame_buffer[row * window_width + col] = background_color;
			}
		}
	}
}

void draw_line_vec2(vec2_t a, vec2_t b, uint32_t stroke_color) {
	int x1 = (int)a.x;
	int x2 = (int)b.x;
	int y1 = (int)a.y;
	int y2 = (int)b.y;

	draw_line(x1, y1, x2, y2, stroke_color);
}

void draw_line(int x1, int y1, int x2, int y2, uint32_t stroke_color) {
	int delta_x = abs(x2 - x1);
	int delta_y = abs(y2 - y1);

	int longest_side = delta_x > delta_y ? delta_x : delta_y;
	
	float x_inc = (x2 - x1) / (float)longest_side;
	float y_inc = (y2 - y1) / (float)longest_side;

	float x = x1;
	float y = y1;

	for (int i = 0; i < longest_side; i++) {
		if (x > window_width || y > window_height || x < 0 || y < 0) {
			break;
		}
		draw_pixel(round(x), round(y), stroke_color);
		x += x_inc;
		y += y_inc;
	}
}

void draw_rect(int x, int y, int width, int height, uint32_t fill_color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			draw_pixel(x + i, j + y, fill_color);
		}
	}
}

void draw_pixel(int x, int y, uint32_t color)
{
	if (x < 0 || x >= window_width || y < 0 || y >= window_height) {
		return;
	}

	frame_buffer[y * window_width + x] = color;
}

void clear_frame_buffer(uint32_t color) {
	for (int i = 0; i < window_width * window_height; i++) {
		frame_buffer[i] = color;
	}
}

void render_frame_buffer(void) {
	SDL_UpdateTexture(
		texture,
		NULL, // NULL for source rectangle
		frame_buffer,
		window_width * sizeof(uint32_t)
	);

	// NULL for source and destination rectangles
	// because we want to render the entire texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);
}

void cleanup(void) {
	free(frame_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
