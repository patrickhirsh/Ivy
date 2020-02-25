#version 460 core

layout(location = 0) out vec4 color;

in vec3 fPosition;
uniform samplerCube fCubemap;

void main()
{
	color = texture(fCubemap, fPosition);
}