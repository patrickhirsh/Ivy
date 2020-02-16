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
		struct MetaData
		{
			MetaData(
				bool VertexPositions,
				bool VertexNormals,
				bool VertexTextures,
				GLuint BufferSize,
				GLuint BufferCount)
				:
				VertexPositions(VertexPositions),
				VertexNormals(VertexNormals),
				VertexTextures(VertexTextures),
				BufferSize(BufferSize),
				BufferCount(BufferCount) {}

			bool VertexPositions;
			bool VertexNormals;
			bool VertexTextures;
			GLuint BufferSize;
			GLuint BufferCount;
			Ivy::Ref<cy::GLRenderTexture<GL_TEXTURE_2D>> RT;
		};

		StaticMeshResource					(std::string sourcePath);
		~StaticMeshResource					() {};
		Ivy::Ref<MetaData> GetMetaData		() const;
		bool IsLoaded						() const { return _loaded; }
		void Load							();
		void Unload							();
		bool Bind							();
		void Unbind							();

		// TODO: Generate a draw call struct containing draw call info

	private:
		bool								_loaded;
		std::string							_source;
		bool								_hasVertexPositions = false;
		bool								_hasVertexNormals = false;
		bool								_hasVertexTextures = false;
		std::vector<float>					_buffer;
		GLuint								_texture;
		unsigned char*						_textureData;
		int									_textureWidth;
		int									_textureHeight;
		int									_textureNRChannels;
		Ivy::Ref<VertexBuffer>				_vb = nullptr;
		Ivy::Ref<VertexArray>				_va = nullptr;
		VertexBufferLayout					_vbLayout;
	};
}