#pragma once
#include <SDL.h>

// global declarations
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* frame_buffer;
extern SDL_Texture* texture;
extern int window_width;
extern int window_height;

// function signatures 
bool initialize_window(void);
void cleanup(void);
void draw_grid(int size, uint32_t grid_color, uint32_t background_color);
void draw_rect(int x, int y, int width, int height, uint32_t fill_color);
void clear_frame_buffer(uint32_t color);
void render_frame_buffer(void);

