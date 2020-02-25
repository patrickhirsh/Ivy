#pragma once
#include "Core/IvyPCH.h"
#include "Render/Shader.h"
#include "Component/Component.h"

namespace _Ivy
{
	class Resource
	{
	public:
		// TODO: implement singleton + internal pattern to control initialization
		// TODO: use generic-type binding and auto-unbind on a new bind call
		static std::string LoadShader									(std::string shaderPath);
		static std::vector<Ivy::Ref<Shader>>							_shaders;
		static Ivy::Ref<Shader>											_vertexShader;
		static Ivy::Ref<Shader>											_fragmentShader;
		static std::vector<Ivy::Ref<Shader>>							_shadersCM;
		static Ivy::Ref<Shader>											_vertexShaderCM;
		static Ivy::Ref<Shader>											_fragmentShaderCM;
	};
}