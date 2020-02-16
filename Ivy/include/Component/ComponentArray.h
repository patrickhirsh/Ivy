#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"

namespace _Ivy
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray				() {};
		virtual void EntityDestroyed			(Entity entity) = 0;
	};

	template<typename Component>
	class ComponentArray : public IComponentArray
	{
	public:
		void AddComponent						(Entity entity, Component component);
		void RemoveComponent					(Entity entity);
		Component& GetComponent					(Entity entity);
		void EntityDestroyed					(Entity entity) override;
	private:
		std::array<Component, MAX_ENTITIES>		_components;
		std::unordered_map<Entity, size_t>		_entityToIndex;
		std::unordered_map<size_t, Entity>		_indexToEntity;
		size_t									_size;
	};
}
