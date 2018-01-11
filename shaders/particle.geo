#version 150

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 clr_in[];
out vec3 clr;
out vec2 uv;

void main (void)
{
  mat4 MV = model * view;
  float size = gl_in[0].gl_Position.w;

  vec3 right = vec3(MV[0][0],
                    MV[1][0],
                    MV[2][0]);

  vec3 up = vec3(MV[0][1],
                 MV[1][1],
                 MV[2][1]);

  vec3 P = gl_in[0].gl_Position.xyz;

  mat4 VP = projection * view;

  vec3 va = P - (right + up) * size;
  gl_Position = VP * vec4(va, 1.0);
  clr = clr_in[0];
  uv.x = 0;
  uv.y = 0;
  EmitVertex();

  vec3 vb = P - (right - up) * size;
  gl_Position = VP * vec4(vb, 1.0);
  clr = clr_in[0];
  uv.x = 1;
  uv.y = 0;
  EmitVertex();

  vec3 vd = P + (right - up) * size;
  gl_Position = VP * vec4(vd, 1.0);
  clr = clr_in[0];
  uv.x = 0;
  uv.y = 1;
  EmitVertex();

  vec3 vc = P + (right + up) * size;
  gl_Position = VP * vec4(vc, 1.0);
  clr = clr_in[0];
  uv.x = 1;
  uv.y = 1;
  EmitVertex();

  clr = clr_in[0];
  EndPrimitive();
}

