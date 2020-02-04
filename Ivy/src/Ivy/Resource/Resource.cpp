#include "Core/IvyPCH.h"
#include "Resource/Resource.h"

namespace Ivy
{
	std::unordered_map<std::string, Resource::MetaResource<Ivy::StaticMesh, StaticMeshResource>*> Resource::_resourcePoolStaticMesh;

	Ivy::Ref<Shader>			Resource::_vertexShader = nullptr;
	Ivy::Ref<Shader>			Resource::_fragmentShader = nullptr;
	

	template<typename ObjectType, typename ResourceType>
	Resource::MetaResource<ObjectType, ResourceType>::MetaResource(Ivy::Ref<ResourceType> resource)
		: _resource(resource) {}

	template<typename ObjectType, typename ResourceType>
	bool Resource::MetaResource<ObjectType, ResourceType>::HasReferences()
	{
		// TODO: Clear invalid references and return the resulting ref count
		return true;
	}

	template<typename ObjectType, typename ResourceType>
	const Ivy::Ref<ResourceType> Resource::MetaResource<ObjectType, ResourceType>::GetResource(Ivy::WeakRef<ObjectType> requester)
	{
		ObjectType* requestingObject = requester.lock().get();
		_references.emplace(requestingObject, requester);
		return _resource;
	}

	std::string Resource::LoadShader(std::string shaderPath)
	{
		std::string fullPath = Ivy::GetResourceDirectory() + shaderPath;
		std::ifstream file(shaderPath);
		if (!file) { /* ERROR.. */ return ""; }

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}

	Ivy::Ref<StaticMeshResource::MetaData> Resource::BindStaticMesh(Ivy::WeakRef<Ivy::StaticMesh> staticMesh)
	{
		MetaResource<Ivy::StaticMesh, StaticMeshResource>* resourceMeta;
		auto it = _resourcePoolStaticMesh.find(staticMesh.lock()->GetMeshPath());
		if (it != _resourcePoolStaticMesh.end()) { resourceMeta = it->second; }
		else
		{
			// load new static mesh - resource isn't in the pool
			auto resource = Ivy::Ref<StaticMeshResource>(new StaticMeshResource(staticMesh.lock()->GetMeshPath()));
			resourceMeta = new MetaResource<Ivy::StaticMesh, StaticMeshResource>(resource);
			_resourcePoolStaticMesh.emplace(staticMesh.lock()->GetMeshPath(), resourceMeta);
		}

		Ivy::Ref<StaticMeshResource> resource = resourceMeta->GetResource(staticMesh);
		return resource->Bind() ? resource->GetMetaData() : nullptr;
	}

	void Resource::UnbindStaticMesh(Ivy::WeakRef<Ivy::StaticMesh> staticMesh)
	{
		auto it = _resourcePoolStaticMesh.find(staticMesh.lock()->GetMeshPath());
		if (it != _resourcePoolStaticMesh.end())
		{
			it->second->GetResource(staticMesh)->Unbind();
		}
	}
}