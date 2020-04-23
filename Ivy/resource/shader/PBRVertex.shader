#version 460 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec3 vTangent;
layout(location = 3) in vec2 vTexCoord;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 NormalTransform;

out vec3 fPosition;
out vec3 fNormal;
out mat4 fNormalTransform;
out mat3 fTBN;
out vec2 fTexCoord;

void main()
{
	gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
	fPosition = vec3(View * Model * vec4(vPosition, 1.0));
	fNormal = vec3(NormalTransform * vec4(vNormal, 1.0));
	fNormalTransform = NormalTransform;

	// Create TBN matrix to compute tangent space
	vec3 T = normalize(vec3(Model * vec4(vTangent, 0.0)));
	vec3 N = normalize(vec3(Model * vec4(vNormal, 1.0)));
	vec3 B = cross(N, T);
	fTBN = mat3(T, B, N);

	fTexCoord = vTexCoord;

	//fNormal = vec3(NormalTransform * vec4(texture(NormalMap, vTexCoord).rgb, 1.0));
}