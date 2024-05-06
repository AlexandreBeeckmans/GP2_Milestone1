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
layout(location = 5) in vec3 fragModelPosition;

layout(location = 0) out vec4 outColor;


void main() 
{
    const vec3 lightDirection = normalize(vec3(0.0, 1.0, 1.0));
    const float lightIntensity = 5.0f;
    const float shininess = 25.0f;

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

    float observedArea = dot(-lightDirection, finalNormal);



    vec3 specularValue = vec3(texture(specularSampler, fragUV));
    vec3 reflection = reflect(-lightDirection, finalNormal);

    //vec3 invViewDirection = vec3(0,0,1);
    vec3 invViewDirection = normalize(fragCameraPosition - fragModelPosition);


	//If cosine is lower than zero we take a 0 value
    float cosinus = max(0.0f, dot(reflection, invViewDirection));
    vec3 specularReflection =
	vec3(
        specularValue.r * pow(cosinus, shininess),
		specularValue.g * pow(cosinus, shininess),
		specularValue.b * pow(cosinus, shininess)
	);
    

	//avoid negative value for specular reflection
    specularReflection.x = max(0.0f, specularReflection.r);
    specularReflection.y = max(0.0f, specularReflection.g);
    specularReflection.z = max(0.0f, specularReflection.b);








    // Output color
    if(observedArea > 0)
    {

        vec3 diffuseSpecularColor = vec3(texture(texSampler, fragUV)) + specularReflection * lightIntensity;
        vec3 finalColor = diffuseSpecularColor * observedArea;

        // Calculate the dot product between the normal and the light direction
        //float diff = max(dot(finalNormal, lightDirection), 0.2);

        // Simple diffuse lighting
        //vec4 diffuse = diff * texture(texSampler, fragUV);
        outColor = vec4(finalColor,1);
    }
    else
    {
        outColor = vec4(0,0,0,1);
    }
    

    
}