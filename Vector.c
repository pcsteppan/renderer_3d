#include <math.h>
#include "Vector.h"

vec3_t vec3_rotate(vec3_t v, vec3_t rot) {
	return vec3_rotate_x(vec3_rotate_y(vec3_rotate_z(v, rot.z), rot.y), rot.x);
}

vec3_t vec3_rotate_x(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		v.x,	
		v.y * cos(angle) - v.z * sin(angle),
		v.y * sin(angle) + v.z * cos(angle)
	};
	return rotated_vector;
}

vec3_t vec3_rotate_y(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		v.x * cos(angle) - v.z * sin(angle),
		v.y,
		v.x * sin(angle) + v.z * cos(angle),
	};
	return rotated_vector;
}


vec3_t vec3_rotate_z(vec3_t v, float angle) {
	vec3_t rotated_vector = {
		v.x * cos(angle) - v.y * sin(angle),
		v.x * sin(angle) + v.y * cos(angle),
		v.z
	};
	return rotated_vector;
}
