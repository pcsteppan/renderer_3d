#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

bool is_running;
SDL_Window* window;
SDL_Renderer* renderer;

bool initialize_window(void) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}
	
	int x, y;
	x = y = SDL_WINDOWPOS_CENTERED;
	// create SDL window
	window = SDL_CreateWindow(NULL, x, y, 800, 600, SDL_WINDOW_BORDERLESS);

	if (!window) {
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

void setup(void) {
	// TODO
}

void process_input(void) {
	// TODO
}

void update(void) {
	// TODO
}

void render(void) {
	// TODO
}

int main(int argc, char* args[]) {
	is_running = initialize_window();

	setup();

	while (is_running) {
		process_input();
		update();
		render();
	}

	return 0;
}

