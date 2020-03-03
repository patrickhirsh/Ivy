#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalTransform;

uniform sampler2D NormalMap;

out vec3 fPosition;
out vec3 fNormal;
out vec2 fTexCoord;

void main()
{
	gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
	fPosition = vec3(View * Model * vec4(vPosition, 1.0));
	fTexCoord = vTexCoord;

	fNormal = vec3(NormalTransform * vec4(texture(NormalMap, vTexCoord).rgb, 1.0));
}