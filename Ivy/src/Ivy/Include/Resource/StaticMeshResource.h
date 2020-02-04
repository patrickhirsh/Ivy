#pragma once
#include "Core/IvyPCH.h"
#include "Window/Render/Shader.h"
#include "Window/Render/VertexBuffer.h"
#include "Window/Render/VertexBufferLayout.h"
#include "Window/Render/VertexArray.h"
#include "Window/Render/IndexBuffer.h"

namespace Ivy
{
	class StaticMeshResource
	{
	public:
		struct MetaData
		{
			MetaData(
				bool VertexPositions,
				bool VertexNormals,
				GLuint BufferSize,
				GLuint BufferCount)
				:
				VertexPositions(VertexPositions),
				VertexNormals(VertexNormals),
				BufferSize(BufferSize),
				BufferCount(BufferCount) {}

			bool VertexPositions;
			bool VertexNormals;
			GLuint BufferSize;
			GLuint BufferCount;
		};

		StaticMeshResource				(std::string sourcePath);
		~StaticMeshResource				() {};
		Ivy::Ref<MetaData> GetMetaData	() const;
		bool IsLoaded					() const { return _loaded; }
		void Load						();
		void Unload						();
		bool Bind						();
		void Unbind						();

		// TODO: Generate a draw call struct containing draw call info

	private:
		bool						_loaded;
		std::string					_source;
		bool						_hasVertexPositions = false;
		bool						_hasVertexNormals = false;
		std::vector<float>			_buffer;
		Ivy::Ref<VertexBuffer>		_vb = nullptr;
		Ivy::Ref<VertexArray>		_va = nullptr;
		VertexBufferLayout			_vbLayout;
	};
}