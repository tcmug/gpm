#version 330

uniform vec3 light_world_pos;
uniform float light_far_plane;
uniform vec3 viewer_position;

in vec4 world_pos;

void main() {
	float distance_to_light = distance(light_world_pos, world_pos.xyz);
	gl_FragDepth = distance_to_light / light_far_plane;
}
