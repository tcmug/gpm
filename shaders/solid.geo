#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform sampler2D diffuse_texture;
uniform vec3 viewer_position;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


in vec2 uv_v[];

out vec2 uv;
out vec3 world_pos;

void main() {
  for (int i = 0; i < 3; i++) {
	gl_Position = projection * view * model * gl_in[i].gl_Position;
	uv = uv_v[i];
	world_pos = (view * model * gl_in[i].gl_Position).xyz;
	EmitVertex();
  }
  EndPrimitive();
}
