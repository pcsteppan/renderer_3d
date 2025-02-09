#include <stdio.h>
#include "Vector.h"
#include "Mesh.h"
#include "Array.h"

mesh_t mesh = {
	NULL, NULL, {0,0,0}
};

/*
        G----E    6----4
       /|   /|   /|   /|
      B----C |  1----2 |
      | H--|-F  | 7--|-5
      |/   |/   |/   |/ 
      A----D    0----3   
*/

vec3_t cube_vertices[N_CUBE_VERTICES] = {
	{-1, -1, -1}, // A 0
	{-1,  1, -1}, // B 1
	{ 1,  1, -1}, // C 2
	{ 1, -1, -1}, // D 3
	{ 1,  1,  1}, // E 4
	{ 1, -1,  1}, // F 5
	{-1,  1,  1}, // G 6
	{-1, -1,  1}  // H 7
};

face_t cube_faces[N_CUBE_FACES] = {
	// front
	{0, 1, 2},
	{2, 3, 0},
	// right
	{3, 2, 4},
	{4, 5, 3},
	// back
	{5, 4, 6},
	{6, 7, 5},
	// left
	{7, 6, 1},
	{1, 0, 7},
	// top
	{1, 6, 4},
	{4, 2, 1},
	// bottom
	{7, 0, 3},
	{3, 5, 7}
};

void load_cube_mesh_data(void) {
	for (int i = 0; i < N_CUBE_VERTICES; i++) {
		vec3_t vertex = cube_vertices[i];
		array_push(mesh.vertices, vertex);
	}

	for (int i = 0; i < N_CUBE_FACES; i++) {
		face_t face = cube_faces[i];
		array_push(mesh.faces, face);
	}
}
