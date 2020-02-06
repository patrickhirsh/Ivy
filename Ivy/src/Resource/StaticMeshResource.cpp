#include "Core/IvyPCH.h"
#include "Resource/StaticMeshResource.h"

namespace _Ivy
{
	StaticMeshResource::StaticMeshResource(std::string sourcePath)
		: _source(sourcePath), _loaded(false) {}

	Ivy::Ref<StaticMeshResource::MetaData> StaticMeshResource::GetMetaData() const
	{
		return Ivy::Ref<MetaData>( new MetaData(
			_hasVertexPositions,
			_hasVertexNormals,
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
		_hasVertexPositions = mesh.NV() > 0;
		_hasVertexNormals = mesh.NVN() > 0;
		if (_hasVertexPositions) { _vbLayout.Push<float>(3); }
		if (_hasVertexNormals) { _vbLayout.Push<float>(3); }

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
			}
		}

		// generate vertex array & vertex buffer with vertices
		_va = VertexArray::Create();
		_vb = VertexBuffer::Create(_buffer.data(), _buffer.size() * sizeof(float));
		_va->SetVertexBuffer(_vb, _vbLayout);
		_loaded = true;
	}

	void StaticMeshResource::Unload()
	{
		_vb = nullptr;
		_va = nullptr;
		_buffer.clear();
		_hasVertexPositions = false;
		_hasVertexNormals = false;
		_loaded = false;
	}

	bool StaticMeshResource::Bind()
	{
		if (!_loaded) { Load(); }
		if (_loaded)
		{
			_vb->Bind();
			_va->Bind();
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
		}
	}
}