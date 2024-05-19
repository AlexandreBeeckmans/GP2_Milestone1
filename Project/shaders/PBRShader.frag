#version 450

layout(binding = 1) uniform sampler2D texSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D specularSampler;
layout(binding = 4) uniform sampler2D glossSampler;



layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec3 fragTangent;

layout(location = 4) in vec3 fragCameraPosition;
layout(location = 5) in vec4 fragVertexPosition;

layout(location = 0) out vec4 outColor;


void main() 
{
    const vec3 lightDirection = vec3(0.577f, -0.577f, 0.577f);
    const float lightIntensity = 7.0f;
    const float shininess = 5.0f;
    const vec3 ambientLighting = vec3(0.025f, 0.025f, 0.025f);

    vec3 binormal = cross(fragNormal, fragTangent);
        
    mat3x3 tangentSpaceAxis = mat3x3( normalize(fragTangent),normalize(binormal),normalize(fragNormal));
    
    //sample the normal from normal map and place in interval [-1;1]
    vec3 sampledNormal = normalize(vec3(texture(normalSampler, fragUV)));
    sampledNormal *= 2.0f;
    sampledNormal.x -= 1.0f;
    sampledNormal.y -= 1.0f;
    sampledNormal.z -= 1.0f;
    
    vec3 finalNormal =  tangentSpaceAxis * normalize(sampledNormal);;

    float observedArea = dot(-lightDirection, finalNormal);



    vec3 specularValue = vec3(texture(specularSampler, fragUV));
    vec3 reflection = reflect(-lightDirection, finalNormal);

    vec3 invViewDirection = normalize(fragCameraPosition - fragVertexPosition.xyz);


	//If cosine is lower than zero we take a 0 value
    float cosinus = max(0.0f, dot(reflection, invViewDirection));
    vec3 specularReflection =
	vec3(
        specularValue.x * pow(cosinus, (shininess * texture(glossSampler, fragUV).r)),
		specularValue.y * pow(cosinus, (shininess * texture(glossSampler, fragUV).r)),
		specularValue.z * pow(cosinus, (shininess * texture(glossSampler, fragUV).r))
	);
    

	//avoid negative value for specular reflection
    specularReflection.x = max(0.0f, specularReflection.x);
    specularReflection.y = max(0.0f, specularReflection.y);
    specularReflection.z = max(0.0f, specularReflection.z);


    // Output color
    if(observedArea > 0)
    {

        vec3 diffuseSpecularColor = vec3(texture(texSampler, fragUV)) + specularReflection * lightIntensity;

        vec3 finalColor = diffuseSpecularColor * observedArea + ambientLighting;

        outColor = vec4(finalColor,1);
        //outColor = vec4(observedArea, observedArea, observedArea, 1);
    }
    else
    {
        outColor = vec4(0,0,0,1);
    }

    //outColor = vec4(vec3(texture(texSampler, fragUV)),1);
}