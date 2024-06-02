#version 450

layout(binding = 1) uniform sampler2D albedoSampler;
layout(binding = 2) uniform sampler2D normalSampler;
layout(binding = 3) uniform sampler2D roughnessSampler;
layout(binding = 4) uniform sampler2D metallicSampler;

layout(push_constant) uniform Constants
{
    layout(offset = 76) int useNormalMap;
    layout(offset = 80) int useDiffuseMap;
    layout(offset = 84) int useRoughnessMap;
    layout(offset = 88) int useMetallicMap;
} constant;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;

layout(location = 2) in vec2 fragUV;
layout(location = 3) in vec3 fragTangent;

layout(location = 4) in vec3 fragCameraPosition;
layout(location = 5) in vec4 fragVertexPosition;

layout(location = 0) out vec4 outColor;

const float PI = 3.14159265358979323846f;

vec3 Lambert(vec3 kd, vec3 cd)
{
			return vec3
			(
				kd.r * cd.r / PI,
				kd.g * cd.g / PI,
				kd.b * cd.b / PI
			);
}


vec3 FresnelFunction_Schlick(vec3 h, vec3 v, const vec3 f0)
{
    return f0 + (vec3(1,1,1) - f0) * pow(1 - (dot(h, v)),5);
}

float NormalDistribution_GGX(vec3 n, vec3 h, float roughness)
{
			float roughnessSquared = roughness * roughness;
            roughnessSquared = roughnessSquared * roughnessSquared;

			const float a = pow(max(0, dot(n, h)), 2);
			const float b = roughnessSquared - 1;
			return roughnessSquared / (PI * pow(a * b + 1, 2));
}

float GeometryFunction_SchlickGGX(vec3 n, vec3 v, float roughness)
{
			float k = pow((roughness * roughness + 1),2) / 8;
			return max(0,dot(n,v)) / (max(0,dot(n,v)) * (1 - k) + k);
}

float GeometryFunction_Smith(vec3 n, vec3 v, vec3 l, float roughness)
{
    return GeometryFunction_SchlickGGX(n,v,roughness) * GeometryFunction_SchlickGGX(n,l,roughness);
}

vec3 CalculateMappedNormal()
{
    vec3 binormal = cross(fragNormal, fragTangent);
        
    mat3x3 tangentSpaceAxis = mat3x3( normalize(fragTangent),normalize(binormal),normalize(fragNormal));
    
    //sample the normal from normal map and place in interval [-1;1]
    vec3 sampledNormal = normalize(vec3(texture(normalSampler, fragUV)));
    sampledNormal *= 2.0f;
    sampledNormal.x -= 1.0f;
    sampledNormal.y -= 1.0f;
    sampledNormal.z -= 1.0f;
    
    return tangentSpaceAxis * normalize(sampledNormal);
}


void main()
{
    const vec3 lightDirection = vec3(-0.577f, -0.577f, 0.577f);
    vec3 lightColor = vec3(1,1,1);
    float lightIntensity = 3.0f;

    vec3 finalAlbedo;
    finalAlbedo = vec3(texture(albedoSampler, fragUV));
    float finalRoughness;
    finalRoughness = texture(roughnessSampler, fragUV).r;
    float finalMetallic = texture(metallicSampler, fragUV).r;

    vec3 finalNormal = CalculateMappedNormal();

    vec3 viewDirection = normalize(fragCameraPosition - vec3(fragVertexPosition));
    vec3 halfViewLight = normalize(((lightDirection - viewDirection) * 0.5f));
    //vec3 halfViewLight = normalize(viewDirection + lightDirection);

    vec3 f0 = vec3(0.04f, 0.04f, 0.04f);
    f0 = mix(f0, finalAlbedo, finalMetallic);


    vec3 F = FresnelFunction_Schlick(halfViewLight, viewDirection, f0);
	float D = NormalDistribution_GGX(finalNormal, halfViewLight, finalRoughness);
	float G = GeometryFunction_Smith(finalNormal, viewDirection, lightDirection, finalRoughness);

    vec3 specular = (D * G * F)/ (4 * max(0, dot(viewDirection, finalNormal)) * max(0, dot(lightDirection, finalNormal)) );
    //vec3 specular = D * G * F;
    specular.r = max(0, specular.r);
    specular.g = max(0, specular.g);
    specular.b = max(0, specular.b);


    vec3 kd = vec3(1.0, 1.0, 1.0) - F;
    kd = kd * (1.0 - finalMetallic);
	vec3 diffuse = Lambert(kd, finalAlbedo);


    float observedArea = dot(finalNormal, lightDirection);
    vec3 irradiance = lightColor * lightIntensity;
    vec3 ambientLight = vec3(0.03f, 0.03f, 0.03f);

    if(observedArea > 0)
    {
        //outColor = vec4(((diffuse + specular)) + ambientLight, 1.0f);
    }
    else
    {
        outColor = vec4(0,0,0,1);
    }

    outColor = vec4((diffuse + specular) * observedArea * irradiance,1);

    

    
    
}