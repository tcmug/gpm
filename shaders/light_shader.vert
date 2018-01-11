#version 330

uniform mat4 projection;
uniform mat4 world;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec2 texture_uv;
layout (location = 2) in vec3 normal;
//layout (location = 3) in vec3 tangent;

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
    //vec3 tangent;
} vs_out;

void main() {

	vec4 vertex4 = vec4(vertex, 1);
    gl_Position = projection * world * vertex4;

    vs_out.position = (world * vertex4).xyz;
    vs_out.normal = normal;
    vs_out.uv = texture_uv;
 //   vs_out.tangent = tangent;

}
