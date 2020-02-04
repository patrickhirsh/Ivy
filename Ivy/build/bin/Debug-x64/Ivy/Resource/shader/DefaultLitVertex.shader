#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 NTRANS;

out vec3 fPosition;
out vec3 fNormal;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	fPosition = vec3(view * model * vec4(vPosition, 1.0));
	fNormal = vec3(NTRANS * vec4(vNormal, 1.0));
}