#include "Core/IvyPCH.h"
#include "Resource/Resource.h"

namespace _Ivy
{
	Ivy::Ref<Shader>			Resource::_vertexShader = nullptr;
	Ivy::Ref<Shader>			Resource::_fragmentShader = nullptr;

	std::string Resource::LoadShader(std::string shaderPath)
	{
		std::string fullPath = _Ivy::GetResourceDirectory() + shaderPath;
		std::ifstream file(shaderPath);
		if (!file) { /* ERROR.. */ return ""; }

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}
}