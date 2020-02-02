#pragma once
#include "Core/IvyPCH.h"
#include "Render/Shader.h"
#include "Render/VertexBuffer.h"
#include "Render/VertexBufferLayout.h"
#include "Render/VertexArray.h"
#include "Render/IndexBuffer.h"

namespace _Ivy
{
	class StaticMeshResource
	{
	public:
		StaticMeshResource			(std::string sourcePath);
		~StaticMeshResource			() {};
		GLuint GetIndeceCount		() const { return _indeces.size(); }
		bool IsLoaded				() const { return _loaded; }
		void Load					();
		void Unload					();
		bool Bind					();
		void Unbind					();

	private:
		bool						_loaded;
		std::string					_source;
		std::vector<float>			_vertices;
		std::vector<unsigned int>	_indeces;
		Ivy::Ref<VertexBuffer>		_vb = nullptr;
		Ivy::Ref<VertexArray>		_va = nullptr;
		Ivy::Ref<IndexBuffer>		_ib = nullptr;
		VertexBufferLayout			_vbLayout;
	};
}