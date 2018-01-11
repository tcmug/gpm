#ifndef SHADER_HPP
#define SHADER_HPP

#include "common.hpp"

/*
	Class: shader
	   Shader class, consists of a vertex, geometry and a fragment shader
*/
class shader {

	public:

		/*
			Variable: shader
				Currently active shader
		*/
		static shader *active;

		/*
			Variable: geometry
				OpenGL geometry shader
		*/
		GLuint geometry;

		/*
			Variable: vertex
				OpenGL vertex shader
		*/

		GLuint vertex;
		/*
			Variable: fragment
				OpenGL fragment shader
		*/
		GLuint fragment;

		/*
			Variable: program
				OpenGL program
		*/
		GLuint program;

		/*
			Function: shader
				Constructor
		*/
		shader(const char *vertex_name, const char *geometry_name, const char *fragment_name);


		/*
			Function: ~shader
				Destructor
		 */
		~shader();

		/*
			Function: enable
				Enable this shader
		*/
		void enable();

		/*
			Function: disable
				Disable this shader
		*/
		void disable();


		/*
			Function: var_loc
				Get uniform location for given name

			Parameters:
				name - Name of the variable

			Returns:
				GLuint uniform location
		*/
		GLuint var_loc(const char *name);

	private:

		/*
			Function: load
				Load an opengl shader file

			Parameters:
				filename - Name of the file
				type - Type of the shader to load

			Returns:
				GLuint
		*/
		GLuint load(const char *filename, int type);
};



#endif
