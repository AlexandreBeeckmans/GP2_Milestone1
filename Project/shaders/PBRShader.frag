#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;


void main() 
{
    const vec3 lightDirection = normalize(vec3(0.0, -1.0, -1.0));

    // Calculate the dot product between the normal and the light direction
    float diff = max(dot(fragNormal, lightDirection), 0.2);

    // Simple diffuse lighting
    vec4 diffuse = diff * texture(texSampler, fragUV);


    // Output color
    outColor = diffuse;

    
}