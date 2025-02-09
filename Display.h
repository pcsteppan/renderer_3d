#pragma once
#include <SDL.h>
#include "Vector.h"

#define FPS 30 
#define FRAME_TARGET_TIME (1000/FPS)

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
void draw_pixel(int x, int y, uint32_t color);
void clear_frame_buffer(uint32_t color);
void render_frame_buffer(void);

void draw_line(int x1, int y1, int x2, int y2, uint32_t stroke_color);
void draw_line_vec2(vec2_t a, vec2_t b, uint32_t stroke_color);

