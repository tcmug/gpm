#version 330

layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 clr_in;


void main()
{
    gl_Position = model * vertex;
    clr_in = color;
}
