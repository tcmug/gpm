#version 330

uniform sampler2D diffuse_texture;

in vec3 clr;
in vec2 uv;
out vec4 color;

void main()
{
	color = texture(diffuse_texture, uv);
    //color = vec4(clr, 1);
}
