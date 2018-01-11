#ifndef FBO_HPP
#define FBO_HPP

#include "common.hpp"
#include "shader.hpp"

class shader;

/*
	Class: fbo
		Frame buffer object class. This defines a texture/buffer which can be
		use as a rendering target.
*/
class fbo {


	public:

		enum {
			COLOR = 1,
			DEPTH = 2,
			CUBE = 4,
			COLOR_AND_DEPTH = COLOR | DEPTH
		};

		/*
			Variable: textureID
				OpenGL texture ID
		*/
		GLuint textureID;

		/*
			Variable: fb
				OpenGL target fb
		*/
		GLuint fb;

		/*
			Variable: depth_rb
				Depth render buffer
		*/
		GLuint depth_rb;

		/*
			Variable: depth_textureID
				Depth buffers texture ID
		*/
		GLuint depth_textureID;

		/*
			Variable: width
				Width of the frame buffer
		*/
		int width;

		/*
			Variable: height
				Height of the frame buffer
		*/
		int height;

		/*
			Variable: mode
				Frame bufferm ode
		*/
		int mode;

		/*
			Constructor: fbo
				No param constructor.
		*/
		fbo() {}

		/*
			Constructor: fbo
				Constructor

			Parameters:
				w - width of the fbo
				h - height of the fbo
				mode - Color and depth settings
		*/
		fbo(int w, int h, int mode = COLOR_AND_DEPTH);

		/*
			Function: render_start
				Begin rendering to this fbo.
		*/
		void render_start();

		/*
			Function: render_end
				End rendering to this fbo. Returns the rendering to the screen.
		*/
		void render_end();

		/*
			Function: blit
				Blit (copy) this FBO to the given one

			Parameters:
				other - Target FBO
				x - Target x Coordinate
				y - Target y coordinate
				w - Width of the area to copy on to
				h - Height of the are to copy on to
		*/
		void blit(fbo *other, int x, int y, int w, int h);

		/*
			Function: blit_smooth
				Perform a smooth blit of this FBO to the given one

			Parameters:
				other - Target FBO
				x - Target x Coordinate
				y - Target y coordinate
				w - Width of the area to copy on to
				h - Height of the are to copy on to
		*/
		void blit_smooth(fbo *other, int x, int y, int w, int h);

		/*
			Function: blit
				Perform a smooth blit of this FBO to the given one using the
				specified shader

			Parameters:
				other - Target FBO
				shd - Shader to use
				x - Target x Coordinate
				y - Target y coordinate
				w - Width of the area to copy on to
				h - Height of the are to copy on to
		*/
		void blit_shader(fbo *other, shader *shd, int x, int y, int w, int h);

		void test_box();


		void gl_bind(int index);
};




#endif
