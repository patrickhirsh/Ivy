#include "Core/IvyPCH.h"
#include "Resource/StaticMeshResource.h"

// STB: https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

namespace _Ivy
{
	StaticMeshResource::StaticMeshResource(std::string sourcePath)
		: _source(sourcePath), _loaded(false) {}

	Ivy::Ref<StaticMeshResource::MetaData> StaticMeshResource::GetMetaData() const
	{
		return Ivy::Ref<MetaData>( new MetaData(
			_hasVertexPositions,
			_hasVertexNormals,
			_hasVertexTextures,
			_buffer.size() * sizeof(float),
			(_buffer.size() * sizeof(float)) / _vbLayout.GetStride()));
	}

	void StaticMeshResource::Load()
	{
		std::string path = _Ivy::GetResourceDirectory() + _source;
		cy::TriMesh mesh = cy::TriMesh();
		if (!mesh.LoadFromFileObj(path.c_str()))
		{
			LOG_ERROR("failed to load static mesh from path: " << path);
			return;
		}

		// build vb layout based on mesh data
		_hasVertexPositions =		mesh.NV() > 0;
		_hasVertexNormals =			mesh.NVN() > 0;
		_hasVertexTextures =		mesh.NVT() > 0;
		if (_hasVertexPositions)	{ _vbLayout.Push<float>(3); } // xp, yp, zp
		if (_hasVertexNormals)		{ _vbLayout.Push<float>(3); } // xn, yn, zn
		if (_hasVertexTextures)		{ _vbLayout.Push<float>(3); } // xt, yt, zt

		// for each face...
		for (int face = 0; face < mesh.NF(); face++)
		{
			// for each vertex...
			for (int vertexIndex = 0; vertexIndex < 3; vertexIndex++)
			{
				// add vertex position coords
				if (_hasVertexPositions)
				{
					if (mesh.F(face).v[vertexIndex] >= mesh.NV())
					{
						LOG_ERROR("Couldn't load mesh: " << _source << " (invalid vertex position data)");
						ASSERT(false);
						Unload();
						return;
					}
					_buffer.push_back(mesh.V(mesh.F(face).v[vertexIndex])[0]);
					_buffer.push_back(mesh.V(mesh.F(face).v[vertexIndex])[1]);
					_buffer.push_back(mesh.V(mesh.F(face).v[vertexIndex])[2]);
				}

				// add vertex normal coords
				if (_hasVertexNormals)
				{
					if (mesh.FN(face).v[vertexIndex] >= mesh.NVN())
					{
						LOG_ERROR("Couldn't load mesh: " << _source << " (invalid vertex normal data)");
						ASSERT(false);
						Unload();
						return;
					}
					_buffer.push_back(mesh.VN(mesh.FN(face).v[vertexIndex])[0]);
					_buffer.push_back(mesh.VN(mesh.FN(face).v[vertexIndex])[1]);
					_buffer.push_back(mesh.VN(mesh.FN(face).v[vertexIndex])[2]);
				}

				// add vertex texture coords
				if (_hasVertexTextures)
				{
					if (mesh.FT(face).v[vertexIndex] >= mesh.NVT())
					{
						LOG_ERROR("Couldn't load mesh: " << _source << " (invalid vertex texture data)");
						ASSERT(false);
						Unload();
						return;
					}
					_buffer.push_back(mesh.VT(mesh.FT(face).v[vertexIndex])[0]);
					_buffer.push_back(mesh.VT(mesh.FT(face).v[vertexIndex])[1]);
					_buffer.push_back(mesh.VT(mesh.FT(face).v[vertexIndex])[2]);
				}
			}
		}

		// generate vertex array & vertex buffer with vertices
		_va = VertexArray::Create();
		_vb = VertexBuffer::Create(_buffer.data(), _buffer.size() * sizeof(float));
		_va->SetVertexBuffer(_vb, _vbLayout);

		// texture
		_textureData = stbi_load((GetResourceDirectory() + "brick.png").c_str(), &_textureWidth, &_textureHeight, &_textureNRChannels, STBI_rgb_alpha);
		GL(glGenTextures(1, &_texture));

		_loaded = true;
	}

	void StaticMeshResource::Unload()
	{
		_vb = nullptr;
		_va = nullptr;
		_buffer.clear();
		_hasVertexPositions = false;
		_hasVertexNormals = false;
		_hasVertexTextures = false;
		_texture = 0;
		stbi_image_free(_textureData);
		_textureData = nullptr;
		_loaded = false;
	}

	bool StaticMeshResource::Bind()
	{
		if (!_loaded) { Load(); }
		if (_loaded)
		{
			_vb->Bind();
			_va->Bind();

			// texture
			GL(glBindTexture(GL_TEXTURE_2D, _texture));
			GL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _textureWidth, _textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _textureData));
			GL(glGenerateMipmap(GL_TEXTURE_2D));
			return true;
		}
		return false;
	}

	void StaticMeshResource::Unbind()
	{
		if (_loaded)
		{
			_vb->Unbind();
			_va->Unbind();
			GL(glBindTexture(GL_TEXTURE_2D, 0));
		}
	}
}