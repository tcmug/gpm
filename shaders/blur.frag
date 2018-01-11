#version 330


uniform sampler2D diffuse_texture;

in vec2 uv;
out vec4 color;

void main()
{

	vec2 v_blurTexCoords[14];
    vec2 v_texCoord = uv;

	v_blurTexCoords[ 0] = v_texCoord + vec2(0.0, -0.028);
	v_blurTexCoords[ 1] = v_texCoord + vec2(-0.024, 0.0);
	v_blurTexCoords[ 2] = v_texCoord + vec2(0.0, -0.020);
	v_blurTexCoords[ 3] = v_texCoord + vec2(-0.016, 0.0);
	v_blurTexCoords[ 4] = v_texCoord + vec2(0.0, -0.012);
	v_blurTexCoords[ 5] = v_texCoord + vec2(-0.008, 0.0);
	v_blurTexCoords[ 6] = v_texCoord + vec2(0.0, -0.004);
	v_blurTexCoords[ 7] = v_texCoord + vec2(0.004, 0.0);
	v_blurTexCoords[ 8] = v_texCoord + vec2(0.0,  0.008);
	v_blurTexCoords[ 9] = v_texCoord + vec2(0.012, 0.0);
	v_blurTexCoords[10] = v_texCoord + vec2(0.0,  0.016);
	v_blurTexCoords[11] = v_texCoord + vec2(0.020, 0.0);
	v_blurTexCoords[12] = v_texCoord + vec2(0.0,  0.024);
	v_blurTexCoords[13] = v_texCoord + vec2(0.028, 0.0);

	color = vec4(0.0);
    color += texture(diffuse_texture, v_blurTexCoords[ 0])*0.0044299121055113265;
    color += texture(diffuse_texture, v_blurTexCoords[ 1])*0.00895781211794;
    color += texture(diffuse_texture, v_blurTexCoords[ 2])*0.0215963866053;
    color += texture(diffuse_texture, v_blurTexCoords[ 3])*0.0443683338718;
    color += texture(diffuse_texture, v_blurTexCoords[ 4])*0.0776744219933;
    color += texture(diffuse_texture, v_blurTexCoords[ 5])*0.115876621105;
    color += texture(diffuse_texture, v_blurTexCoords[ 6])*0.147308056121;
    color += texture(diffuse_texture, v_texCoord         )*0.159576912161;
    color += texture(diffuse_texture, v_blurTexCoords[ 7])*0.147308056121;
    color += texture(diffuse_texture, v_blurTexCoords[ 8])*0.115876621105;
    color += texture(diffuse_texture, v_blurTexCoords[ 9])*0.0776744219933;
    color += texture(diffuse_texture, v_blurTexCoords[10])*0.0443683338718;
    color += texture(diffuse_texture, v_blurTexCoords[11])*0.0215963866053;
    color += texture(diffuse_texture, v_blurTexCoords[12])*0.00895781211794;
    color += texture(diffuse_texture, v_blurTexCoords[13])*0.0044299121055113265;

}



