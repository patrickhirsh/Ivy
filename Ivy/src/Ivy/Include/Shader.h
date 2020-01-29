#pragma once
#include "IvyPCH.h"

namespace _Ivy
{
	class Shader
	{
	public:
		static Ivy::Ref<Shader> Create			(GLuint shaderType, std::string shaderPath);
		static void Bind						(const std::vector<Ivy::Ref<Shader>>& shaders);
		static void Unbind						();
		static GLuint GetActiveProgram			() { return _activeProgram; }
	private:
		static GLuint							_activeProgram;
		static std::vector<Ivy::Ref<Shader>>	_activeShaders;

	public:
		~Shader									() {};
		void Compile							();
	private:
		Shader									() {};
		GLuint									_id;
		GLuint									_type;
		std::string								_path;
	};
}