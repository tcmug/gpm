#ifndef VERTEX_ARRAY_HPP
#define VERTEX_ARRAY_HPP

#include "common.hpp"

#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/*
	Class: gl_buffer
		Base class for buffers
*/
class gl_buffer {

	public:

		/*
			Variable: id
				OpenGL ID for this buffer
		*/
		GLuint id;

		/*
			Variable: usage
				OpenGL usage (GL_STREAM_DRAW etc.)
		*/
		GLenum usage;

		/*
			Constructor: gl_buffer
				Base constructor, takes the usage as parameter

			Parameters:
				u - OpenGL usage
		*/
		gl_buffer(GLenum u = GL_STREAM_DRAW);

		/*
			Desctructor: gl_buffer
			Base destructor
		*/
		~gl_buffer();
};



/*
	Class: glm_vec2_buffer
		A buffer for holding glm::vec2 data
*/
class glm_vec2_buffer: public gl_buffer {

	public:
		/*
			Variable: data
				std::vector which holds the v3 vertices
		*/
		std::vector <glm::vec2> data;

		/*
			Function: gl_vao_bind
				Bind this v3 to the given shader index

			Parameters:
				index - Shader index number

		*/
		void gl_vao_bind(int index);

		/*
			Function: gl_update
				Description

			Parameters:
				size - Limit number of vertices, -1 for the whole buffer
		*/
		void gl_update(int size = -1);
};


/*
	Class: glm_vec3_buffer
		A buffer for holding glm::vec3 data
*/
class glm_vec3_buffer: public gl_buffer {

	public:
		/*
			Variable: data
				std::vector which holds the v3 vertices
		*/
		std::vector <glm::vec3> data;

		/*
			Function: gl_vao_bind
				Bind this v3 to the given shader index

			Parameters:
				index - Shader index number

		*/
		void gl_vao_bind(int index);

		/*
			Function: gl_update
				Description

			Parameters:
				size - Limit number of vertices, -1 for the whole buffer
		*/
		void gl_update(int size = -1);
};





/*
	Class: glm_vec4_buffer
		A buffer for holding glm::vec4 data
*/
class glm_vec4_buffer: public gl_buffer {

	public:
		/*
			Variable: data
				std::vector which holds the v3 vertices
		*/
		std::vector <glm::vec4> data;

		/*
			Function: gl_vao_bind
				Bind this v3 to the given shader index

			Parameters:
				index - Shader index number

		*/
		void gl_vao_bind(int index);

		/*
			Function: gl_update
				Description

			Parameters:
				size - Limit number of vertices, -1 for the whole buffer
		*/
		void gl_update(int size = -1);
};






#endif
