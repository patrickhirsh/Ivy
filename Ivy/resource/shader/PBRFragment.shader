#version 460 core

out vec4		Color;
in vec3			fPosition;
in vec3			fNormal;
//in vec2		fTexCoord;

// Material Parameters
const vec3		albedo = vec3(1.0, 0.2, 0.1);
uniform float   metallic;
uniform float   roughness;
uniform float   ao;

const int		LIGHT_COUNT = 2;
const float		PI = 3.14159265359;
const float		FRESNEL_F0_CONSTANT = 0.04;
const float		AMBIENT_CONSTANT = 0.03;

const vec3		CameraPosition = vec3(0.0, 0.0, 0.0);
//uniform vec3	CameraPosition;

// Lights
const vec3		lightPositions[LIGHT_COUNT] = vec3[](vec3(5.0, 5.0, -20.0), vec3(-15.0, 0.0, -20.0));
const vec3		lightColors[LIGHT_COUNT]	= vec3[](vec3(255.0, 255.0, 255.0), vec3(200, 10, 10));


float DistributionTRGGX(vec3 N, vec3 H, float roughness);
float GeometrySGGX(float NdotV, float roughness);
float GeometrySSGGX(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

void main()
{
    vec3 N = normalize(fNormal);
    vec3 V = normalize(CameraPosition - fPosition);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - fPosition);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - fPosition);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // cook-torrance brdf
        float NDF = DistributionTRGGX(N, H, roughness);
        float G = GeometrySSGGX(N, V, L, roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = numerator / max(denominator, 0.001);

        // add to outgoing radiance Lo
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

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