#pragma once
#include "Vector.h"
#include "Triangle.h"

/*
        G----E    6----4
       /|   /|   /|   /|
      B----C |  1----2 |
      | H--|-F  | 7--|-5
      |/   |/   |/   |/ 
      A----D    0----3   
*/

#define N_CUBE_VERTICES 8
extern vec3_t cube_vertices[N_CUBE_VERTICES];

#define N_CUBE_FACES 12
extern face_t cube_faces[N_CUBE_FACES];

void load_cube_mesh_data(void);
void load_obj_file_data(char* filename);

typedef struct {
    vec3_t* vertices; // dynamic array of vertices
    face_t* faces;
    vec3_t rotation; // euler angles
    vec3_t scale;
    vec3_t translation;
} mesh_t;

extern mesh_t mesh;
