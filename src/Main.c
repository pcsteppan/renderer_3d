#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "Display.h"
#include "Vector.h"
#include "Mesh.h"
#include "Array.h"

bool is_running;

float fov_factor = 128 * 8;
vec3_t camera_pos = { 0, 0, 0 };

int prev_frame_time = 0;
float t = 0;

triangle_t* triangle_render_buffer = NULL;

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

	clear_frame_buffer(0xFF000000);
	array_hold(triangle_render_buffer, 1000, sizeof(triangle_t));
	load_cube_mesh_data();
	mesh.rotation = (vec3_t) { 0.0001,0.0002,0.0003 };
	// load_obj_file_data("./assets/f22.obj");
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

vec2_t project_vec3(vec3_t v) {
	v.z -= camera_pos.z;

 	vec2_t pt_2d = { v.x * fov_factor / v.z, v.y * fov_factor / v.z };

 	pt_2d.x += window_width / 2;
	pt_2d.y += window_height / 2;

	vec2_t v2 = { pt_2d.x, pt_2d.y };
	return v2;
}

void draw_vec(vec3_t v) {
	vec2_t v2 = project_vec3(v);
	draw_rect(v2.x, v2.y, 4, 4, 0xFFFFFFFF);
}

void draw() {
	for (int i = 0; i < array_length(triangle_render_buffer); i++) {
		triangle_t tri = triangle_render_buffer[i];

		draw_line_vec2(tri.points[0], tri.points[1], 0xFFFFFFFF);
		draw_line_vec2(tri.points[1], tri.points[2], 0xFFFFFFFF);
		draw_line_vec2(tri.points[2], tri.points[0], 0xFFFFFFFF);
	}
}

void update(void) {
	array_empty(triangle_render_buffer);
	mesh.rotation = (vec3_t) { 0.5 , -0.0004 * t, 3.24 };
	
	for (int i = 0; i < array_length(mesh.faces); i++) {
		face_t face = mesh.faces[i];
		
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[face.a];
		face_vertices[1] = mesh.vertices[face.b];
       		face_vertices[2] = mesh.vertices[face.c];

		vec3_t transformed_vertices[3];

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = vec3_rotate(face_vertices[i], mesh.rotation);
			transformed_vertex.z -= camera_pos.z;
			transformed_vertices[j] = transformed_vertex;
		}
		
		vec3_t v_a = transformed_vertices[0];	
		vec3_t v_b = transformed_vertices[1];	
		vec3_t v_c = transformed_vertices[2];

		vec3_t v_ab = vec3_sub(v_b, v_a);
		vec3_t v_ac = vec3_sub(v_c, v_a);	
		
		// Compute normal -- left-handed coordinate system
		// defines order of vectors
		vec3_t normal = vec3_cross(v_ab, v_ac);

		vec3_t v_cama = vec3_sub(camera_pos, v_a);

		// Compute dot product between normal and camera ray 'cama'
		float dot = vec3_dot(normal, v_cama);

		// If dot > 0 then the face is facing the camera so we keep
		// Otherwise we cull 
		if (dot <= 0) {
			continue;
		}

		triangle_t projected_triangle;
		for (int j = 0; j < 3; j++){
			vec2_t projected_vertex = project_vec3(transformed_vertices[j]);
			projected_triangle.points[j] = projected_vertex;	
		}
		
		array_push(triangle_render_buffer, projected_triangle);
	}
}

void render(void) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	// clear_frame_buffer(0xFF000000);
	draw_grid(window_width / 20, 0xFF222222, 0xFF000000);
	draw();
	render_frame_buffer();

	SDL_RenderPresent(renderer);
}

void free_resources(void) {
	array_free(triangle_render_buffer);
	array_free(mesh.faces);
	array_free(mesh.vertices);
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

		t = (float)SDL_GetTicks();
		
		process_input();
		update();
		render();
	}

	cleanup();
	free_resources();

	return 0;
}
