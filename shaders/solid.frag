#version 330

uniform sampler2D diffuse_texture;
uniform vec3 viewer_position;
uniform mat4 projection;
uniform mat4 world;

in vec2 uv;
in vec3 world_pos;

out vec4 color;

void main() {
	color = texture(diffuse_texture, uv);
	//color = vec4(world_pos, 1);
	//color = vec4(length(viewer_position - world_pos) / 100);
}

