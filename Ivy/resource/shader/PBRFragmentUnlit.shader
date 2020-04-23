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

void main()
{
	Color = vec4(0.8, 0.0, 0.0, 1.0);
}