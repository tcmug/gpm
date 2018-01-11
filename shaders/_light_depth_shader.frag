// THIS IS OK

#version 330

uniform sampler2D diffuse_texture;

in vec2 uv;
out vec4 color;

void main()
{
    color = texture(diffuse_texture, uv);
}
/*
#version 330

#extension GL_EXT_gpu_shader4 : require

varying vec4 position_cs;
varying vec3 normal_cs;
varying vec3 color;

uniform mat4x4 camera_view_matrix_inv;
uniform mat4x4 light_view_matrix;
uniform mat4x4 light_projection_matrix;
uniform samplerCubeShadow shadow;
uniform vec3 light_position;

void main() {
	vec4 position_ls = light_view_matrix * camera_view_matrix_inv * position_cs;

	// shadow map test
	vec4 abs_position = abs(position_ls);
	float fs_z = -max(abs_position.x, max(abs_position.y, abs_position.z));
	vec4 clip = light_projection_matrix * vec4(0.0, 0.0, fs_z, 1.0);
	float depth = (clip.z / clip.w) * 0.5 + 0.5;
	vec4 result = shadowCube(shadow, vec4(position_ls.xyz, depth));
	vec3 lvector = light_position - position_cs.xyz;
	float ldistance = length(lvector);
	float lintensity = max(dot(normal_cs, normalize(lvector)), 0.0) * 10.0;
	lintensity /= ldistance * ldistance;
	lintensity /= lintensity + 0.5;
	vec3 diffuse = lintensity * result.xyz * color;
	// gl_FragColor = gl_FragColor + vec4(diffuse,1);
	gl_FragColor = vec4(diffuse, 1) * 0.5;
}
*/
