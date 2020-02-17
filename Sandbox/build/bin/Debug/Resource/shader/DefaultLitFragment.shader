#version 460 core

layout(location = 0) out vec4 color;

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoord;

uniform sampler2D textureData;

void main()
{
	vec3 objectColor = vec3(1.0, 0.2, 0.1);
	vec3 lightPosition = vec3(10f, 5f, -20f);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	
	// ambient
	float ambientIntensity = 0.2;
	vec3 ambient = ambientIntensity * lightColor;

	// diffuse
	vec3 norm = normalize(fNormal);
	vec3 lightDir = normalize(lightPosition - fPosition);
	float diffuseIntensity = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffuseIntensity * lightColor;

	//specular
	float specularIntensity = 1.0;
	vec3 viewDir = normalize(-fPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularIntensity * spec * lightColor;

	color = vec4((ambient + diffuse + specular), 1.0) * texture(textureData, fTexCoord);
}