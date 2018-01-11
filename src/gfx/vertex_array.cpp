
#include "vertex_array.hpp"

#include <assert.h>
#include <iostream>

gl_buffer::gl_buffer(GLenum u) {
	glGenBuffers(1, &this->id);
	this->usage = u;
}


gl_buffer::~gl_buffer() {
	glDeleteBuffers(1, &this->id);
	engine_gl_check();
}


void glm_vec2_buffer::gl_vao_bind(int index) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, this->usage);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, 0);
	engine_gl_check();
}


void glm_vec2_buffer::gl_update(int size) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * (size == -1 ? this->data.size() : size), &this->data[0], this->usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	engine_gl_check();
}





void glm_vec3_buffer::gl_vao_bind(int index) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, this->usage);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
	engine_gl_check();
}


void glm_vec3_buffer::gl_update(int size) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * (size == -1 ? this->data.size() : size), &this->data[0], this->usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	engine_gl_check();
}



void glm_vec4_buffer::gl_vao_bind(int index) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, this->usage);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, 0);
	engine_gl_check();
}


void glm_vec4_buffer::gl_update(int size) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * (size == -1 ? this->data.size() : size), &this->data[0], this->usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	engine_gl_check();
}

