#pragma once
#include "Core/IvyPCH.h"
#include "Render/Shader.h"
#include "Entity/StaticMesh.h"
#include "Resource/StaticMeshResource.h"

namespace Ivy
{
	class Resource
	{
		// TODO: auto-GC resources with no remaining references
		template<typename ObjectType, typename ResourceType>
		class MetaResource
		{
		public:
			MetaResource												(Ivy::Ref<ResourceType> resource);
			~MetaResource												() {};
			bool HasReferences											();
			const Ivy::Ref<ResourceType> GetResource					(Ivy::WeakRef<ObjectType> requester);
		private:
			std::unordered_map<ObjectType*, Ivy::WeakRef<ObjectType>>	_references;
			Ivy::Ref<ResourceType>										_resource;
		};

		template<typename ObjectType, typename ResourceType>
		using ResourcePool = std::unordered_map<std::string, MetaResource<ObjectType, ResourceType>*>;

	public:
		// TODO: use generic-type binding and auto-unbind on a new bind call
		static std::string LoadShader									(std::string shaderPath);
		static Ivy::Ref<StaticMeshResource::MetaData> BindStaticMesh	(Ivy::WeakRef<Ivy::StaticMesh> staticMesh);
		static void UnbindStaticMesh									(Ivy::WeakRef<Ivy::StaticMesh> staticMesh);
		static Ivy::Ref<Shader>											_vertexShader;
		static Ivy::Ref<Shader>											_fragmentShader;
	private:

		static ResourcePool<Ivy::StaticMesh, StaticMeshResource>		_resourcePoolStaticMesh;
	};
}