
#include "voxels.hpp"

#include <iostream>

#include "../gfx/resource.hpp"

#include <glm/glm.hpp>

// Create a new voxel set
//
//
voxel_mesh::voxel_mesh() {

	int total;

	this->size = 1 << BLOCK_SHIFT_SIZE;

	total = (sizeof(GLfloat) * 11) * 6 * 6 * this->size * this-> size * this->size;

	this->vao = 0;

	this->solid_count = 0;
	this->num_vertices = -1;

	this->vertex_data.data.reserve(this->size * this->size * this->size * 3 * 2 * 6);
	this->uv_data.data.reserve(this->size * this->size * this->size * 3 * 2 * 6);
	this->normal_data.data.reserve(this->size * this->size * this->size * 3 * 2 * 6);
	this->tangent_data.data.reserve(this->size * this->size * this->size * 3 * 2 * 6);

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	this->vertex_data.gl_vao_bind(0);
	this->uv_data.gl_vao_bind(1);
	this->normal_data.gl_vao_bind(2);
		//this->tangent_data.gl_vao_bind(3);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	engine_gl_check();


	printf("%ix%ix%i, total: %i bytes\n", this->size, this->size, this->size, total);

	this->total_size = this->size * this->size * this->size;
	this->data = (unsigned char *)malloc(this->total_size * sizeof(unsigned char));

	memset(this->data, 0, this->total_size);

	this->min_x = this->size;
	this->min_y = this->size;
	this->min_z = this->size;

	this->max_x = 0;
	this->max_y = 0;
	this->max_z = 0;


}



// Free voxels
//
//
voxel_mesh::~voxel_mesh() {

	free(this->data);
}


// Set the given voxel in the set
//
//
unsigned char voxel_mesh::put(int x, int y, int z, unsigned char s) {
	unsigned char current = 0;
	if (x >= 0 && x < this->size && y >= 0 && y < this->size && z >= 0 && z < this->size) {

		int ii = x + (y * this->size) + (z * this->size * this->size);
		int i = x + (y << BLOCK_SHIFT_SIZE) + (z << (BLOCK_SHIFT_SIZE*2));

		assert(ii == i);
		assert(i >= 0);
		assert(i < this->total_size);

		current = this->data[i];

		if (current != s) {

			if (x < this->min_x) this->min_x = x;
			if (y < this->min_y) this->min_y = y;
			if (z < this->min_z) this->min_z = z;

			if (x > this->max_x) this->max_x = x;
			if (y > this->max_y) this->max_y = y;
			if (z > this->max_z) this->max_z = z;

			if (s) {
				this->solid_count++;
			} else {
				this->solid_count--;
			}
			this->data[i] = s;
			this->num_vertices = -1; // signal that the mesh needs updating
		}


	}
	return current;
}



// Get the given voxel from the set (raw call without boundary check)
//
//
unsigned char voxel_mesh::_get(int x, int y, int z) {

	return this->data[x + (y << BLOCK_SHIFT_SIZE) + (z << (BLOCK_SHIFT_SIZE*2))];

}



// Get given voxel in the set
//
//
unsigned char voxel_mesh::get(int x, int y, int z) {

	if (x >= 0 && x < this->size && y >= 0 && y < this->size && z >= 0 && z < this->size) {
		return  this->_get(x, y, z);
	}

	return 0;
}



// Fill a box
//
//
void voxel_mesh::fill_box(int x, int y, int z, int w, int h, int d, unsigned char s) {
	x -= w / 2;
	y -= h / 2;
	z -= d / 2;
	while (d >= 0) {
		int th = h;
		while (th >= 0) {
			int tw = w;
			while (tw >= 0) {
				this->put(x+tw, y+th, z+d, s);
				tw--;
			}
			th--;
		}
		d--;
	}
}


glm::vec3 get_tangent(glm::vec3 vert1, glm::vec3 vert2, glm::vec3 vert3, glm::vec2 st1, glm::vec2 st2, glm::vec2 st3) {

	glm::vec3 vec1, vec2;

	vec1 = vert3 - vert1;
	vec2 = vert2 - vert1;

	float d_u1 = st3.x - st1.x;
	float d_u2 = st2.y - st1.y;

	glm::vec3 tangent = (vec1 * d_u1) - (vec2 * d_u2);

	return glm::normalize(tangent);;
	// glm::vec3 normal = vert1.cross(vert2);

	// float coef = 1/ (st1.x * st2.y - st2.x * st1.y);
	// glm::vec3 tangent;

	// tangent.x = coef * ((vert1.x * st2.y)  + (vert2.x * -st1.y));
	// tangent.y = coef * ((vert1.y * st2.y)  + (vert2.y * -st1.y));
	// tangent.z = coef * ((vert1.z * st2.y)  + (vert2.z * -st1.y));

//	return tangent;
	//glm::vec3 binormal = normal.cross(tangent);
}

// Render the voxel set
//
//
void voxel_mesh::render() {

	if (this->num_vertices == -1) {
		this->_update();
	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, this->num_vertices);
	glBindVertexArray(0);

}

// Update mesh of the voxel set
//
//
void voxel_mesh::_update() {

	int x, y, z;

	unsigned long start_time = SDL_GetTicks();

	this->num_vertices = 0;

	for (z = 0; z < this->size; z++) {
		for (y = 0; y < this->size; y++) {
			for (x = 0; x < this->size; x++) {

				unsigned char self, left, right, top, bottom, front, back;

				if (0 == (self = this->get(x, y, z))) {
					continue;
				}

				float ty = self / 16.0f;
				float tx = ty - floor(ty);

				ty = floor(floor(ty) / 16.0f);

#define _A 	x,   y+1, z+1
#define _B 	x+1, y+1, z+1
#define _C 	x+1, y,   z+1
#define _D 	x,   y,   z+1

#define _E 	x+1, y+1, z
#define _F 	x,   y+1, z
#define _G 	x,   y,   z
#define _H 	x+1, y,   z

#define _T00 tx, ty
#define _T01 tx, ty+1.0/16
#define _T10 tx+1.0/16, ty
#define _T11 tx+1.0/16, ty+1.0/16

				left   = this->get(x - 1, y, z);
				right  = this->get(x + 1, y, z);
				top    = this->get(x, y + 1, z);
				bottom = this->get(x, y - 1, z);
				front  = this->get(x, y, z + 1);
				back   = this->get(x, y, z - 1);

				if (!left) {

					for (int n = 0; n < 6; n++) {
						this->normal_data.data[this->num_vertices+n] = glm::vec3(-1, 0, 0);
					}

					this->vertex_data.data[this->num_vertices+0] = glm::vec3(_F);
					this->vertex_data.data[this->num_vertices+1] = glm::vec3(_G);
					this->vertex_data.data[this->num_vertices+2] = glm::vec3(_D);
					this->uv_data.data[this->num_vertices+0] = glm::vec2(_T00);
					this->uv_data.data[this->num_vertices+1] = glm::vec2(_T01);
					this->uv_data.data[this->num_vertices+2] = glm::vec2(_T11);

					this->vertex_data.data[this->num_vertices+3] = glm::vec3(_D);
					this->vertex_data.data[this->num_vertices+4] = glm::vec3(_A);
					this->vertex_data.data[this->num_vertices+5] = glm::vec3(_F);
					this->uv_data.data[this->num_vertices+3] = glm::vec2(_T11);
					this->uv_data.data[this->num_vertices+4] = glm::vec2(_T10);
					this->uv_data.data[this->num_vertices+5] = glm::vec2(_T00);

					glm::vec3 tangent = get_tangent(
						this->vertex_data.data[this->num_vertices+0],
						this->vertex_data.data[this->num_vertices+1],
						this->vertex_data.data[this->num_vertices+2],
						this->uv_data.data[this->num_vertices+0],
						this->uv_data.data[this->num_vertices+1],
						this->uv_data.data[this->num_vertices+2]
					);

					for (int n = 0; n < 6; n++) {
						this->tangent_data.data[this->num_vertices+n] = tangent;
					}

					this->num_vertices += 6;

				}

				if (!right) {

					for (int n = 0; n < 6; n++) {
						this->normal_data.data[this->num_vertices+n] = glm::vec3(1, 0, 0);
					}

					this->vertex_data.data[this->num_vertices+0] = glm::vec3(_B);
					this->vertex_data.data[this->num_vertices+1] = glm::vec3(_C);
					this->vertex_data.data[this->num_vertices+2] = glm::vec3(_H);
					this->uv_data.data[this->num_vertices+0] = glm::vec2(_T00);
					this->uv_data.data[this->num_vertices+1] = glm::vec2(_T01);
					this->uv_data.data[this->num_vertices+2] = glm::vec2(_T11);

					this->vertex_data.data[this->num_vertices+3] = glm::vec3(_H);
					this->vertex_data.data[this->num_vertices+4] = glm::vec3(_E);
					this->vertex_data.data[this->num_vertices+5] = glm::vec3(_B);
					this->uv_data.data[this->num_vertices+3] = glm::vec2(_T11);
					this->uv_data.data[this->num_vertices+4] = glm::vec2(_T10);
					this->uv_data.data[this->num_vertices+5] = glm::vec2(_T00);

					glm::vec3 tangent = get_tangent(
						this->vertex_data.data[this->num_vertices+0],
						this->vertex_data.data[this->num_vertices+1],
						this->vertex_data.data[this->num_vertices+2],
						this->uv_data.data[this->num_vertices+0],
						this->uv_data.data[this->num_vertices+1],
						this->uv_data.data[this->num_vertices+2]
					);

					for (int n = 0; n < 6; n++) {
						this->tangent_data.data[this->num_vertices+n] = tangent;
					}

					this->num_vertices += 6;
				}

				if (!top) {

					for (int n = 0; n < 6; n++) {
						this->normal_data.data[this->num_vertices+n] = glm::vec3(0, 1, 0);
					}

					this->vertex_data.data[this->num_vertices+0] = glm::vec3(_F);
					this->vertex_data.data[this->num_vertices+1] = glm::vec3(_A);
					this->vertex_data.data[this->num_vertices+2] = glm::vec3(_B);
					this->uv_data.data[this->num_vertices+0] = glm::vec2(_T00);
					this->uv_data.data[this->num_vertices+1] = glm::vec2(_T01);
					this->uv_data.data[this->num_vertices+2] = glm::vec2(_T11);

					this->vertex_data.data[this->num_vertices+3] = glm::vec3(_B);
					this->vertex_data.data[this->num_vertices+4] = glm::vec3(_E);
					this->vertex_data.data[this->num_vertices+5] = glm::vec3(_F);
					this->uv_data.data[this->num_vertices+3] = glm::vec2(_T11);
					this->uv_data.data[this->num_vertices+4] = glm::vec2(_T10);
					this->uv_data.data[this->num_vertices+5] = glm::vec2(_T00);

					glm::vec3 tangent = get_tangent(
						this->vertex_data.data[this->num_vertices+0],
						this->vertex_data.data[this->num_vertices+1],
						this->vertex_data.data[this->num_vertices+2],
						this->uv_data.data[this->num_vertices+0],
						this->uv_data.data[this->num_vertices+1],
						this->uv_data.data[this->num_vertices+2]
					);

					for (int n = 0; n < 6; n++) {
						this->tangent_data.data[this->num_vertices+n] = tangent;
					}

					this->num_vertices += 6;

				}

				if (!bottom) {

					for (int n = 0; n < 6; n++) {
						this->normal_data.data[this->num_vertices+n] = glm::vec3(0, -1, 0);
					}

					this->vertex_data.data[this->num_vertices+0] = glm::vec3(_D);
					this->vertex_data.data[this->num_vertices+1] = glm::vec3(_G);
					this->vertex_data.data[this->num_vertices+2] = glm::vec3(_H);
					this->uv_data.data[this->num_vertices+0] = glm::vec2(_T00);
					this->uv_data.data[this->num_vertices+1] = glm::vec2(_T01);
					this->uv_data.data[this->num_vertices+2] = glm::vec2(_T11);

					this->vertex_data.data[this->num_vertices+3] = glm::vec3(_H);
					this->vertex_data.data[this->num_vertices+4] = glm::vec3(_C);
					this->vertex_data.data[this->num_vertices+5] = glm::vec3(_D);
					this->uv_data.data[this->num_vertices+3] = glm::vec2(_T11);
					this->uv_data.data[this->num_vertices+4] = glm::vec2(_T10);
					this->uv_data.data[this->num_vertices+5] = glm::vec2(_T00);

					glm::vec3 tangent = get_tangent(
						this->vertex_data.data[this->num_vertices+0],
						this->vertex_data.data[this->num_vertices+1],
						this->vertex_data.data[this->num_vertices+2],
						this->uv_data.data[this->num_vertices+0],
						this->uv_data.data[this->num_vertices+1],
						this->uv_data.data[this->num_vertices+2]
					);

					for (int n = 0; n < 6; n++) {
						this->tangent_data.data[this->num_vertices+n] = tangent;
					}

					this->num_vertices += 6;

				}

				if (!front) {

					for (int n = 0; n < 6; n++) {
						this->normal_data.data[this->num_vertices+n] = glm::vec3(0, 0, 1);
					}

					this->vertex_data.data[this->num_vertices+0] = glm::vec3(_A);
					this->vertex_data.data[this->num_vertices+1] = glm::vec3(_D);
					this->vertex_data.data[this->num_vertices+2] = glm::vec3(_C);
					this->uv_data.data[this->num_vertices+0] = glm::vec2(_T00);
					this->uv_data.data[this->num_vertices+1] = glm::vec2(_T01);
					this->uv_data.data[this->num_vertices+2] = glm::vec2(_T11);

					this->vertex_data.data[this->num_vertices+3] = glm::vec3(_C);
					this->vertex_data.data[this->num_vertices+4] = glm::vec3(_B);
					this->vertex_data.data[this->num_vertices+5] = glm::vec3(_A);
					this->uv_data.data[this->num_vertices+3] = glm::vec2(_T11);
					this->uv_data.data[this->num_vertices+4] = glm::vec2(_T10);
					this->uv_data.data[this->num_vertices+5] = glm::vec2(_T00);

					glm::vec3 tangent = get_tangent(
						this->vertex_data.data[this->num_vertices+0],
						this->vertex_data.data[this->num_vertices+1],
						this->vertex_data.data[this->num_vertices+2],
						this->uv_data.data[this->num_vertices+0],
						this->uv_data.data[this->num_vertices+1],
						this->uv_data.data[this->num_vertices+2]
					);

					for (int n = 0; n < 6; n++) {
						this->tangent_data.data[this->num_vertices+n] = tangent;
					}

					this->num_vertices += 6;
				}

				if (!back) {

					for (int n = 0; n < 6; n++) {
						this->normal_data.data[this->num_vertices+n] = glm::vec3(0, 0, -1);
					}

					this->vertex_data.data[this->num_vertices+0] = glm::vec3(_E);
					this->vertex_data.data[this->num_vertices+1] = glm::vec3(_H);
					this->vertex_data.data[this->num_vertices+2] = glm::vec3(_G);
					this->uv_data.data[this->num_vertices+0] = glm::vec2(_T00);
					this->uv_data.data[this->num_vertices+1] = glm::vec2(_T01);
					this->uv_data.data[this->num_vertices+2] = glm::vec2(_T11);

					this->vertex_data.data[this->num_vertices+3] = glm::vec3(_G);
					this->vertex_data.data[this->num_vertices+4] = glm::vec3(_F);
					this->vertex_data.data[this->num_vertices+5] = glm::vec3(_E);
					this->uv_data.data[this->num_vertices+3] = glm::vec2(_T11);
					this->uv_data.data[this->num_vertices+4] = glm::vec2(_T10);
					this->uv_data.data[this->num_vertices+5] = glm::vec2(_T00);

					glm::vec3 tangent = get_tangent(
						this->vertex_data.data[this->num_vertices+0],
						this->vertex_data.data[this->num_vertices+1],
						this->vertex_data.data[this->num_vertices+2],
						this->uv_data.data[this->num_vertices+0],
						this->uv_data.data[this->num_vertices+1],
						this->uv_data.data[this->num_vertices+2]
					);

					for (int n = 0; n < 6; n++) {
						this->tangent_data.data[this->num_vertices+n] = tangent;
					}

					this->num_vertices += 6;
				}

			}
		}
	}

	unsigned long before_gpu = SDL_GetTicks() - start_time;

	this->vertex_data.gl_update(this->num_vertices);
	this->uv_data.gl_update(this->num_vertices);
	this->normal_data.gl_update(this->num_vertices);
	//this->tangent_data.gl_update();

   // printf("Mesh reconstruct time, cpu: %u, gpu: %u (%i vertices, %i faces)\n", before_gpu, SDL_GetTicks() - start_time, this->num_vertices, this->num_vertices / 6);


}


// Load a voxel mesh
//
//
void voxel_mesh::load(const char *filename) {

}


// Save a voxel mesh
//
//
void voxel_mesh::save(const char *filename) {

}


// void voxels_split(voxels *v) {
// 	int x, y, z;
// 	int objects = 0;
// 	unsigned char mask[BLOCK_SHIFT_SIZE * BLOCK_SHIFT_SIZE * BLOCK_SHIFT_SIZE];
// 	memset(mask, 0, sizeof(mask));
/*

Flood-fill (node, target-color, replacement-color):
 1. If target-color is equal to replacement-color, return.
 2. Set Q to the empty queue.
 3. Add node to the end of Q.
 4. While Q is not empty:
 5.     Set n equal to the last element of Q.
 6.     Remove last element from Q.
 7.     If the color of n is equal to target-color:
 8.         Set the color of n to replacement-color and mark "n" as processed.
 9.         Add west node to end of Q if west has not been processed yet.
 10.        Add east node to end of Q if east has not been processed yet.
 11.        Add north node to end of Q if north has not been processed yet.
 12.        Add south node to end of Q if south has not been processed yet.
 13. Return.

*/
// 	// Locate solid
// 	for (z = 0; z < v->size; z++) {
// 		for (y = 0; y < v->size; y++) {
// 			for (x = 0; x < v->size; x++) {
// 				if (mask[x + (y << BLOCK_SHIFT_SIZE) + (z << (BLOCK_SHIFT_SIZE*2))] == 0 &&
// 					_this->get(x, y, z) > 0) {
// 					mask[x + (y << BLOCK_SHIFT_SIZE) + (z << (BLOCK_SHIFT_SIZE*2))] = objects;
// 				}
// 			}
// 		}
// 	}
// }

