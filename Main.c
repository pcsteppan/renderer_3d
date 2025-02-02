#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

bool is_running;
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
	// TODO
}

draw_grid(int size, uint32_t grid_color, uint32_t background_color) {
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

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	clear_frame_buffer(0xFF000000);
	draw_grid(window_width / 20, 0xFFBBBBBB, 0xFF0000FF);
	render_frame_buffer();

	SDL_RenderPresent(renderer);
}

void cleanup(void) {
	free(frame_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
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

