#include "shader.hpp"



#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


shader *shader::active = 0;


shader::shader(const char *vertex_name, const char *geometry_name, const char *fragment_name) {

	this->geometry = 0;
	this->fragment = 0;
	this->vertex = 0;

	this->program = glCreateProgram();

	if (vertex_name) {
		this->vertex = this->load(vertex_name, GL_VERTEX_SHADER);
		if (this->vertex) {
			glAttachShader(this->program, this->vertex);
			engine_gl_check();
		}
	}

	if (geometry_name) {
		this->geometry = this->load(geometry_name, GL_GEOMETRY_SHADER);
		if (this->geometry) {
			glAttachShader(this->program, this->geometry);
			engine_gl_check();
		}
	}

	if (fragment_name) {
		this->fragment = this->load(fragment_name, GL_FRAGMENT_SHADER);
		if (this->fragment) {
			glAttachShader(this->program, this->fragment);
			engine_gl_check();
		}
	}

	glLinkProgram(this->program);

	GLint success = 0;
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		//We failed to compile.

		printf("Shader linking failed.\n");

		// GLint maxLength = 0;
		// glGetShaderiv(s, GL_INFO_LOG_LENGTH, &maxLength);

		int len = 400000;
		char msg[len];

		// The maxLength includes the NULL character
		//std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(this->program, len, 0, msg);
		std::cout << msg << std::endl;
	}

	engine_gl_check();
}


shader::~shader() {

	if (this->vertex) {
		if (this->program) {
			glDetachShader(this->program, this->vertex);
		}
		glDeleteShader(this->vertex);
	}

	if (this->geometry) {
		if (this->program) {
			glDetachShader(this->program, this->geometry);
		}
		glDeleteShader(this->geometry);
	}

	if (this->fragment) {
		if (this->program) {
			glDetachShader(this->program, this->fragment);
		}
		glDeleteShader(this->fragment);
	}

	if (this->program) {
		glDeleteProgram(this->program);
	}
}


GLuint shader::load(const char *filename, int type) {

	std::ifstream in(filename);
	std::stringstream buffer;
	buffer << in.rdbuf();
	std::string contents(buffer.str());

	if (contents.length() == 0) {
		std::cout << "Shader '" << filename << "' source is empty!" << std::endl;
		return 0;
	}

	GLuint s = glCreateShader(type);

	const char *files[1] = {contents.c_str()};

	glShaderSource(
		s,
		1,
		files,
		NULL
	);

	glCompileShader(s);

	engine_gl_check();

	GLint success = 0;
	glGetShaderiv(s, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		//We failed to compile.

		std::cout << "Shader '" << filename << "' failed compilation." << std::endl;

		// GLint maxLength = 0;
		// glGetShaderiv(s, GL_INFO_LOG_LENGTH, &maxLength);

		int len = 400000;
		char msg[len];

		// The maxLength includes the NULL character
		//std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(s, len, 0, msg);
		std::cout << msg << std::endl;
		glDeleteShader(s);
		return 0;
	}

	engine_gl_check();
	printf("Shader '%s' compiled.\n", filename);
	return s;
}


GLuint shader::var_loc(const char *name) {
	return glGetUniformLocation(this->program, name);
}


void shader::enable() {
	this->active = this;
	glUseProgram(this->program);
	engine_gl_check();
}


void shader::disable() {
	if (this->active == this) {
		this->active = 0;
	}
	glUseProgram(0);
}


