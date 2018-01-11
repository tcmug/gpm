#version 330

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texture_uv;

uniform sampler2D diffuse_texture;
uniform vec3 viewer_position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 uv_v;

void main() {
	gl_Position = vec4(vertex, 1);
	uv_v = texture_uv;
}
