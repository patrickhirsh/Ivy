#include "Core/IvyPCH.h"
#include "Resource/StaticMeshResource.h"

namespace _Ivy
{
	StaticMeshResource::StaticMeshResource(std::string sourcePath)
		: _source(sourcePath), _loaded(false) {}

	void StaticMeshResource::Load()
	{
		std::string path = _Ivy::GetResourceDirectory() + _source;
		cy::TriMesh mesh = cy::TriMesh();
		if (!mesh.LoadFromFileObj(path.c_str()))
		{
			LOG_ERROR("failed to load static mesh from path: " << path);
			return;
		}

		for (int i = 0; i < mesh.NV(); i++)
			for (int j = 0; j < 3; j++)
				_vertices.push_back(mesh.V(i).elem[j]);
		for (int i = 0; i < mesh.NF(); i++)
			for (int j = 0; j < 3; j++)
				_indeces.push_back(mesh.F(i).v[j]);

		_va = VertexArray::Create();
		_vb = VertexBuffer::Create(_vertices.data(), _vertices.size() * sizeof(float));
		_vbLayout.Push<float>(3);
		_va->SetVertexBuffer(_vb, _vbLayout);

		_ib = IndexBuffer::Create(_indeces.data(), _indeces.size());
		_ib->Bind();
		
		_loaded = true;
	}

	void StaticMeshResource::Unload()
	{
		_vb = nullptr;
		_va = nullptr;
		_ib = nullptr;
		_vertices.clear();
		_indeces.clear();
		_loaded = false;
	}

	bool StaticMeshResource::Bind()
	{
		if (!_loaded) { Load(); }
		if (_loaded)
		{
			_vb->Bind();
			_va->Bind();
			_ib->Bind();
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
			_ib->Unbind();
		}
	}
}