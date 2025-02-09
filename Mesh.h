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

#define N_MESH_VERTICES 8
extern vec3_t mesh_vertices[N_MESH_VERTICES];

#define N_MESH_FACES 12
extern face_t mesh_faces[N_MESH_FACES];
