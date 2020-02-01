#pragma once
#include "Core/IvyPCH.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexBufferLayout.h"
#include "Render/VertexArray.h"
#include "Render/IndexBuffer.h"

namespace _Ivy
{
	class Resource
	{
	public:
		static std::string LoadShader(std::string shaderPath);

		/* OBJ Resources*/
		static void AddOBJResource										(std::string objPath);
		static Ivy::Ref<cy::TriMesh> GetOBJResource						(std::string objPath);

	private:
		static std::unordered_map<std::string, Ivy::Ref<cy::TriMesh>>	_objResources;

		// TODO: Proper resource system...
		
	public:
		// Hardcoded Teapot Data
		static std::vector<float>			_vertices;
		static std::vector<unsigned int>	_indeces;
		static Ivy::Ref<VertexArray>		_va;
		static Ivy::Ref<IndexBuffer>		_ib;
		static Ivy::Ref<VertexBuffer>		_vb;
		static VertexBufferLayout			_vbLayout;
		static Ivy::Ref<Shader>				_vertexShader;
		static Ivy::Ref<Shader>				_fragmentShader;
	};
}