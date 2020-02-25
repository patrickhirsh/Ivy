#version 460 core

layout(location = 0) in vec3 vPosition;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 NTRANS;

out vec3 fPosition;

void main()
{
	fPosition = vPosition;
	gl_Position = projection * view * vec4(vPosition, 1.0);
}