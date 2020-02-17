#version 460 core

layout(location = 0) out vec4 color;

//in vec2 fTexCoord;

//uniform sampler2D textureData;

void main()
{
	//color = texture(textureData, fTexCoord);
	color = vec4(1.0, 1.0, 1.0, 1.0);
}