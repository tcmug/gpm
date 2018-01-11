#version 330

uniform mat4 cube_projection[6];
uniform mat4 world;

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

out vec4 world_pos;

void main(void) {
    for (int face = 0; face < 6; face++) {
        gl_Layer = face;
        for (int i = 0; i < 3; i++) {
          world_pos = world * gl_in[i].gl_Position;
          gl_Position = cube_projection[face] * world * gl_in[i].gl_Position;
          EmitVertex();
        }
        EndPrimitive();
    }
}

