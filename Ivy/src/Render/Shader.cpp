#include "Core/IvyPCH.h"
#include "Render/Shader.h"

namespace _Ivy
{
	GLuint Shader::_activeProgram;
	std::vector<Ivy::Ref<Shader>> Shader::_activeShaders;

	Ivy::Ref<Shader> Shader::Create(GLuint shaderType, std::string shaderPath)
	{
		Ivy::Ref<Shader> shader = Ivy::Ref<Shader>(new Shader());
		shader->_id = 0;
		shader->_type = shaderType;
		shader->_path = GetResourceDirectory() + shaderPath;
		shader->Compile();
		return shader;
	}

	void Shader::Bind(const std::vector<Ivy::Ref<Shader>>& shaders)
	{
		Shader::Unbind();
		GL(_activeProgram = glCreateProgram());
		for (Ivy::Ref<Shader> shader : shaders)
		{
			if (shader->_id == 0) { shader->Compile(); }
			GL(glAttachShader(_activeProgram, shader->_id));
			_activeShaders.push_back(shader);
		}
		GL(glLinkProgram(_activeProgram));
		GL(glValidateProgram(_activeProgram));
		GL(glUseProgram(_activeProgram));
	}

	void Shader::Unbind()
	{
		GL(glDeleteProgram(_activeProgram));
		_activeProgram = 0;
		_activeShaders.clear();
	}

	void Shader::Compile()
	{
		// open shader source file
		std::ifstream file(_path);
		if (!file)
		{
			LOG_ERROR("failed to open shader: " << _path);
			return;
		}

		// read shader file
		std::stringstream srcStream;
		srcStream << file.rdbuf();
		file.close();

		// compile shader
		GLuint prevId = _id;
		GL(_id = glCreateShader(_type));
		std::string src = srcStream.str();	// necessary explicit copy
		const char* source = src.c_str();
		GL(glShaderSource(_id, 1, &source, nullptr));
		GL(glCompileShader(_id));

		// ensure successful compilation
		int result;
		GL(glGetShaderiv(_id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GL(glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &length));
			char* message = new char[length];
			glGetShaderInfoLog(_id, length, &length, message);
			LOG_ERROR("Failed to compile shader! (" << message << ") shader: " << _path);
			
			delete[] message;
			GL(glDeleteShader(_id));
			_id = prevId;
		}
	}
}