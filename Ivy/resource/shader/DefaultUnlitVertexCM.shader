#version 460 core

layout(location = 0) in vec3 vPosition;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalTransform;

out vec3 fPosition;

void main()
{
	fPosition = vPosition;
	gl_Position = Projection * View * vec4(vPosition, 1.0);
}