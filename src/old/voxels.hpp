#ifndef VOXELS_HPP
#define VOXELS_HPP


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2_Mixer/SDL_mixer.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>


// #define USE_GL_MAP_BUFFER
// ^^^ CAUSES LAGS
#define BLOCK_SHIFT_SIZE (5)

#include <vector>

#include "../gfx/vertex_array.hpp"

class voxel_mesh {

	public:

		GLuint vao;

		glm_vec3_buffer vertex_data;
		glm_vec3_buffer normal_data;
		glm_vec3_buffer tangent_data;
		glm_vec2_buffer uv_data;

		int num_vertices;
		int solid_count;

		unsigned char *data;

		int total_size;
		int size;

		unsigned char min_x;
		unsigned char min_y;
		unsigned char min_z;

		unsigned char max_x;
		unsigned char max_y;
		unsigned char max_z;

		voxel_mesh();
		~voxel_mesh();

		unsigned char get(int x, int y, int z);
		unsigned char put(int x, int y, int z, unsigned char s);

		void fill_box(int x, int y, int z, int w, int h, int d, unsigned char s);
		void render();

		void load(const char *filename);
		void save(const char *filename);
		void _update();


		unsigned char _get(int x, int y, int z);

};



#endif
