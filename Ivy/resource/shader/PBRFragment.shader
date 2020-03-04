#version 460 core

out vec4			Color;
in vec3				fPosition;
in vec3				fNormal;
in vec2				fTexCoord;

uniform mat4		NormalTransform;

// Material Maps
uniform sampler2D	AlbedoMap;
uniform sampler2D	NormalMap;
uniform sampler2D	MetallicMap;
uniform sampler2D	RoughnessMap;
//uniform sampler2D	AOMap;

// Constants
const float			PI							= 3.14159265359;
const float			FRESNEL_F0_CONSTANT			= 0.04;
const float			AMBIENT_CONSTANT			= 0.01;

// Temporary Constants
const int		LIGHT_COUNT						= 2;
const vec3		LightPositions[LIGHT_COUNT]		= vec3[](vec3(50.0, 5.0, -50.0), vec3(-15.0, 0.0, -20.0));
const vec3		LightColors[LIGHT_COUNT]		= vec3[](vec3(255.0, 255.0, 255.0), vec3(200, 10, 10));
const vec3		CameraPosition					= vec3(0.0, 0.0, 0.0);

float DistributionTRGGX							(vec3 N, vec3 H, float roughness);
float GeometrySGGX								(float NdotV, float roughness);
float GeometrySSGGX								(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick								(float cosTheta, vec3 F0);

void main()
{
	// convert albedo to linear space (there's gotta be a better way to do this...)
	vec3  Albedo		= vec3(pow(texture(AlbedoMap, fTexCoord).r, 2.2), pow(texture(AlbedoMap, fTexCoord).g, 2.2), pow(texture(AlbedoMap, fTexCoord).b, 2.2));
	float Metallic		= texture(MetallicMap, fTexCoord).r;
	float Roughness		= texture(RoughnessMap, fTexCoord).r;

	//vec3 N = normalize(texture(NormalMap, TexCoord).rgb * 2.0 - 1.0);
	vec3 N = normalize(fNormal);

	vec3 V = normalize(CameraPosition - fPosition);

	vec3 F0 = vec3(FRESNEL_F0_CONSTANT);
	F0 = mix(F0, Albedo, Metallic);

	// Compute reflectance over all light sources
	vec3 Lo = vec3(0.0);
	for (int i = 0; i < LIGHT_COUNT; i++)
	{
		// Compute per-light radiance
		vec3 L = normalize(LightPositions[i] - fPosition);
		vec3 H = normalize(V + L);
		float distance = length(LightPositions[i] - fPosition);
		float attenuation = 1.0 / (distance / 5);					
		vec3 radiance = LightColors[i] * attenuation;

		// Compute DGF
		float NDF = DistributionTRGGX(N, H, Roughness);
		float G = GeometrySSGGX(N, V, L, Roughness);
		vec3  F = FresnelSchlick(max(dot(H, V), 0.0), F0);

		// Determine reflection / refraction ratio
		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;
		kD *= 1.0 - Metallic;

		// Compute specular with Cook-Torrance BRDF
		vec3 num = NDF * G * F;
		float denom = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
		vec3 specular = num / max(denom, 0.00001);

		// Sum into outgoing radiance
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * (Albedo / PI) + specular) * radiance * NdotL;
	}

	// Ambient lighting
	vec3 ambient = vec3(AMBIENT_CONSTANT) * Albedo; // * AO;
	vec3 color = ambient + Lo;

	// Reinhard operator (tone-map HDR)
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	Color = vec4(color, 1.0);
}

// Trowbridge-Reitz GGX
float DistributionTRGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return num / denom;
}

// Schlick GGX
float GeometrySGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}

// Smith's Schlick GGX
float GeometrySSGGX(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySGGX(NdotV, roughness);
	float ggx1 = GeometrySGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

// Fresnel-Schlick
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}