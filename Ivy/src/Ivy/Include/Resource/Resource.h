#pragma once
#include "Core/IvyPCH.h"
#include "Render/Shader.h"
#include "Data/Object/Renderable/StaticMesh.h"
#include "Resource/StaticMeshResource.h"

namespace _Ivy
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
		static std::string LoadShader									(std::string shaderPath);
		static Ivy::Ref<StaticMeshResource> BindStaticMesh				(Ivy::WeakRef<Ivy::StaticMesh> staticMesh);
		static Ivy::Ref<Shader>											_vertexShader;
		static Ivy::Ref<Shader>											_fragmentShader;
	private:
		static ResourcePool<Ivy::StaticMesh, StaticMeshResource>		_resourcePoolStaticMesh;
	};
}