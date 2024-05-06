#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec3 fragTangent;

layout(location = 0) out vec4 outColor;


void main() 
{
    const vec3 lightDirection = normalize(vec3(0.0, 1.0, -1.0));
    const float lightIntensity = 1.0f;

    vec3 finalNormal = fragNormal;
    vec3 binormal = cross(fragNormal, fragTangent);
        
    mat3x3 tangentSpaceAxis = mat3x3( normalize(fragTangent),normalize(binormal),normalize(fragNormal));
    
    //sample the normal from normal map and place in interval [-1;1]
    vec3 sampledNormal = vec3(texture(normalSampler, fragUV));
    sampledNormal *= 2.0f;
    sampledNormal.x -= 1.0f;
    sampledNormal.y -= 1.0f;
    sampledNormal.z -= 1.0f;
    
    finalNormal = normalize(normalize(sampledNormal) * tangentSpaceAxis);


    // Calculate the dot product between the normal and the light direction
    float diff = max(dot(finalNormal, lightDirection), 0.2);

    // Simple diffuse lighting
    vec4 diffuse = diff * texture(texSampler, fragUV);
    //vec4 diffuse = vec4(diff * fragColor, 0);


    // Output color
    outColor = diffuse * lightIntensity;

    
}