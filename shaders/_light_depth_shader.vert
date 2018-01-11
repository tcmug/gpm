// THIS IS OK

#version 330

in vec3 vertex;
in vec2 texture_uv;

uniform mat4 projection;
uniform mat4 view;

out vec2 uv;

void main()
{
    gl_Position = projection * view * vec4(vertex, 1);
    uv = texture_uv;
}

/*
#version 330

uniform mat4 projection;
uniform mat4 view;
uniform mat4 inverse_view;

out vec4 position_cs;
out vec3 normal_cs;
out vec3 color;


mat3 mat3_emu(mat4 m4) {
  return mat3(
      m4[0][0], m4[0][1], m4[0][2],
      m4[1][0], m4[1][1], m4[1][2],
      m4[2][0], m4[2][1], m4[2][2]);
}

void main() {
	gl_Position = projection * view * gl_Vertex;
	position_cs = view * gl_Vertex;
	mat4 inv = transpose(inverse(view));
	normal_cs = mat3_emu(inv) * gl_Normal;
	color = gl_Color.rgb;

	// gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	// position_cs = gl_ModelViewMatrix * gl_Vertex;
	// normal_cs = gl_NormalMatrix * gl_Normal;
	// color = gl_Color.rgb;
}
*/
