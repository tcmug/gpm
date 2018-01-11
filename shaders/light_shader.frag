#version 330

uniform samplerCube shadow_cubemap;
uniform sampler2D diffuse_texture;
uniform vec3 light_world_pos;
uniform vec3 viewer_position;
uniform float light_far_plane;

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
//    vec3 tangent;
} fs_in;

out vec4 FragColor;

float ShadowCalculation(vec3 fragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - light_world_pos;
    // Use the light to fragment vector to sample from the depth map
    float closestDepth = texture(shadow_cubemap, fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= light_far_plane;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.1;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    return shadow;
}

void main(void) {

    vec3 color = texture(diffuse_texture, fs_in.uv).rgb;
    vec3 normal = normalize(fs_in.normal);
    vec3 lightColor = vec3(1);
    // Ambient
    vec3 ambient = 0.2 * color;
    // Diffuse
    vec3 lightDir = normalize(light_world_pos - fs_in.position);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    // Specular
    vec3 viewDir = normalize(viewer_position - fs_in.position);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.3;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    // Calculate shadow
    float shadow = ShadowCalculation(fs_in.position);
    vec3 lighting = ((1.0 - shadow) * (diffuse + specular)) * color;

    float dist = distance(light_world_pos, fs_in.position);

    float c_att = 1/light_far_plane;
    float l_att = 1/light_far_plane;
    float q_att = 1/light_far_plane;

    float att = 1.0 / (c_att +
            l_att * dist +
            q_att * dist * dist);

    // float att = 1 - (dist / light_far_plane);
    // lighting = lighting * att;
    lighting = lighting * att + ambient;

    FragColor = vec4(lighting, 1.0f);

}

