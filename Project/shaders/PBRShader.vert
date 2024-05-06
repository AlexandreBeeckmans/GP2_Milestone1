#version 450

layout(set = 0 , binding = 0) uniform UniformBufferObject
{
    mat4 proj;
    mat4 view; 

} ubo;

layout(push_constant)uniform PushConstants
{
    mat4 model;
    vec3 cameraPosition;
} mesh;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

layout(location = 3) in vec2 inUV;
layout(location = 4) in vec3 inTangent;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 fragNormal;

layout(location = 2) out vec2 fragUV;
layout(location = 3) out vec3 fragTangent;

layout(location = 4) out vec3 fragCameraPosition;
layout(location = 5) out vec3 fragModelPosition;

void main() 
{
    gl_Position =  ubo.proj * ubo.view * mesh.model * vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
    vec4 tNormal =  mesh.model*vec4(inNormal,0);
    fragNormal = normalize(tNormal.xyz); // interpolation of normal attribute in fragment shader.
    fragColor = inColor; // interpolation of color attribute in fragment shader.
    fragUV = inUV;


    vec4 tTangent =  mesh.model*vec4(inTangent,0);
    fragTangent = normalize(tTangent.xyz);

    fragCameraPosition = mesh.cameraPosition;
    fragModelPosition = vec3(mesh.model[0][3], mesh.model[1][3], mesh.model[2][3]);
    
}