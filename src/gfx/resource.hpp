#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include "shader.hpp"

void resource_flush();
GLuint resource_load_texture(const char *filename);
shader *resource_load_shader(const char *vertex_name, const char *geometry_name, const char *fragment_name);

#endif
