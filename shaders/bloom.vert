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
