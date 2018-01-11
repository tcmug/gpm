
#include "fbo.hpp"

#include <iostream>


fbo::fbo(int w, int h, int mode) {
	//2D RGBA32 float

	this->width = w;
	this->height = h;
	this->mode = mode;

	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER_EXT, fb);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (this->mode & fbo::CUBE) {

		// CUBE TEXTURE

		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_DEPTH_TEXTURE_MODE, GL_LUMINANCE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		// glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

		this->textureID = -1;
		this->depth_textureID = -1;

		// color cube map
		if (this->mode & fbo::COLOR) {
			glGenTextures(1, &this->textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			for (int face = 0; face < 6; face++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGBA,
					width, height, 0, GL_RGBA, GL_FLOAT, 0);
			}
			glFramebufferTexture(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, this->textureID, 0);
		}
		// depth cube map
		if (this->mode & fbo::DEPTH) {
			glGenTextures(1, &this->depth_textureID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_textureID);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			for (int face = 0; face < 6; face++) {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_DEPTH_COMPONENT24,
					width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
			}
			glFramebufferTexture(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, this->depth_textureID, 0);
		}

		// framebuffer object

		if (this->mode & fbo::COLOR) {
			glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
			engine_gl_check();
		} else {
			glDrawBuffer(GL_NONE);
			engine_gl_check();
			glReadBuffer(GL_NONE);
			engine_gl_check();
		}

	} else {

		// REGULAR 2D
		if (this->mode & fbo::COLOR) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->width, this->height, 0, GL_BGRA, GL_FLOAT, 0);

			// Attach 2D texture to this FBO
			glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureID, 0);
		}

		// Add depth buffer
		if (this->mode & fbo::DEPTH) {
			glGenRenderbuffers(1, &depth_rb);
			glBindRenderbuffer(GL_RENDERBUFFER_EXT, depth_rb);
			glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, this->width, this->height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depth_rb);
		}

	}

	engine_gl_check();

	if (this->mode & fbo::COLOR) {
		glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		engine_gl_check();
	} else {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		engine_gl_check();
	}

	// Check the status of fbo

	GLenum status;
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	switch (status) {

		case GL_FRAMEBUFFER_COMPLETE:
			std::cout << "FBO OK" << std::endl;
		break;

		default:
			std::cout << "FBO creation error: " << status << std::endl;
			exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

}



void fbo::render_start() {
	glViewport(0, 0, (GLsizei)this->width, (GLsizei)this->height);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fb);
}



void fbo::render_end() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



void fbo::blit(fbo *other, int x, int y, int w, int h) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, this->fb);
	// Bind to screen if the other fbo is null
	if (other) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, other->fb);
	} else {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, 0);
	}

	glBlitFramebuffer(
		0,
		0,
		this->width,
		this->height,
		x,
		y,
		x + w,
		y + h,
		GL_COLOR_BUFFER_BIT,
		GL_NEAREST
	);

	// GL_LINEAR
	glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, 0);
}



void fbo::blit_smooth(fbo *other, int x, int y, int w, int h) {
	glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, this->fb);
	// Bind to screen if the other fbo is null
	if (other) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, other->fb);
	} else {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, 0);
	}

	glBlitFramebuffer(
		0,
		0,
		this->width,
		this->height,
		x,
		y,
		x + w,
		y + h,
		GL_COLOR_BUFFER_BIT,
		GL_LINEAR
	);

	// GL_LINEAR
	glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, 0);
}



void fbo::blit_shader(fbo *other, shader *shd, int x, int y, int w, int h) {

	if (other) {
		glBindFramebuffer(GL_FRAMEBUFFER, other->fb);
	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glViewport(x, y, w, h);

	shd->enable();

	// m4 proj, view;
	// proj.identity();
	// proj.projection_orthogonal(0, 1, 0, 1, -1, 1);
	// view.identity();
	// proj.gl_uniform("projection");
	// view.gl_uniform("view");

	// static GLuint gluv = 0, glvert = 0, vao;
	// if (gluv == 0) {

	// 	glGenVertexArrays(1, &vao);

	// 	static GLfloat vertices[] = {
	// 		0, 0, 0,	1, 0, 0,
	// 		0, 1, 0,

	// 		0, 1, 0,	1, 0, 0,
	// 					1, 1, 0
	// 	};

	// 	static GLfloat uv[] = {
	// 		0,0,	1,0,
	// 		0,1,

	// 		0,1,	1,0,
	// 				1,1
	// 	};


	// 	glBindVertexArray(vao);

	// 	glGenBuffers(1, &glvert);
	// 	glBindBuffer(GL_ARRAY_BUFFER, glvert);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 	glEnableVertexAttribArray(0);
	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 	glGenBuffers(1, &gluv);
	// 	glBindBuffer(GL_ARRAY_BUFFER, gluv);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
	// 	glEnableVertexAttribArray(1);
	// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// 	glBindVertexArray(0);
	// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// }

	// glBindAttribLocation(shd->program, 0, "vertex");
	// glBindAttribLocation(shd->program, 1, "texture_uv");
	// glUniform1i(shd->var_loc("diffuse_texture"), 0);

	// glActiveTexture(GL_TEXTURE0);
	// glEnable(GL_TEXTURE_2D);
	// glBindTexture(GL_TEXTURE_2D, this->textureID);

	// glBindVertexArray(vao);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	// glBindVertexArray(0);

	// glDisable(GL_TEXTURE_2D);

	shd->disable();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




void fbo::test_box() {

	glViewport(0, 0, this->width, this->height);

	static shader *flat = NULL;

	if (flat == NULL) {
		flat = new shader("shaders/flat.vert", 0, "shaders/flat.frag");
	}

	flat->enable();

	// m4 proj, view;
	// proj.identity();
	// proj.projection_orthogonal(0, 1, 0, 1, -1, 1);
	// view.identity();
	// proj.gl_uniform("projection");
	// view.gl_uniform("view");

	// static GLuint gluv = 0, glvert = 0, vao;
	// if (gluv == 0) {

	// 	glGenVertexArrays(1, &vao);

	// 	static GLfloat vertices[] = {
	// 		0, 0, 0,	1, 0, 0,
	// 		0, 1, 0,

	// 		0, 1, 0,	1, 0, 0,
	// 					1, 1, 0
	// 	};

	// 	glBindVertexArray(vao);

	// 	glGenBuffers(1, &glvert);
	// 	glBindBuffer(GL_ARRAY_BUFFER, glvert);
	// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 	glEnableVertexAttribArray(0);
	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 	glBindVertexArray(0);
	// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// }

	// glBindAttribLocation(flat->program, 0, "vertex");
	// glBindAttribLocation(flat->program, 1, "texture_uv");
	// glUniform1i(flat->var_loc("diffuse_texture"), 0);

	// glBindVertexArray(vao);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	// glBindVertexArray(0);

	// glDisable(GL_TEXTURE_2D);

	flat->disable();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}





void fbo::gl_bind(int index) {

	glActiveTexture(GL_TEXTURE0 + index);
	if (this->mode & fbo::CUBE) {
		if (this->mode & fbo::COLOR) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureID);
		} else {
			glBindTexture(GL_TEXTURE_CUBE_MAP, this->depth_textureID);
		}
	} else {
		if (this->mode & fbo::COLOR) {
			glBindTexture(GL_TEXTURE_2D, this->textureID);
		} else {
			glBindTexture(GL_TEXTURE_2D, this->depth_textureID);
		}
	}
}


