#version 330


uniform sampler2D diffuse_texture;

in vec2 uv;
out vec4 color;

void main()
{
    color = texture(diffuse_texture, uv);

	if((0.2126*color.r) + (0.7152*color.g) + (0.0722*color.b) > 0.63) {
		float brightenRatio = 1.0 / max(max(color.r, color.g), color.b);
		color.r *= brightenRatio;
		color.g *= brightenRatio;
		color.b *= brightenRatio;
		color = vec4(1.0);
	} else {
		color.r = 0.0;
		color.g = 0.0;
		color.b = 0.0;
	}

}


// #version 330

// uniform sampler2D diffuse_texture;
// uniform float exposure;
// varying vec2 texCoords;


// void main() {
// 	vec4 color = texture2D(diffuse_texture, texCoords);
// 	// float luminance = dot(vec4(0.3, 0.59, 0.11, 0.0), color);
// 	// * exposure;
// 	// color *= luminance;
// 	// if (luminance < exposure) {
// 	// 	color = vec4(0,0,0,0);
// 	// } else {
// 	// 	color *= luminance;
// 	// }


// 	color = color;
// }
