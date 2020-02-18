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
		void AddComponent(Entity entity, Component component)
		{
			// Entity with duplicate components!
			assert(_entityToIndex.find(entity) == _entityToIndex.end());

			size_t newIndex = _size;
			_entityToIndex[entity] = newIndex;
			_indexToEntity[newIndex] = entity;
			_components[newIndex] = component;
			_size++;
		}

		void RemoveComponent(Entity entity)
		{
			// Removing non-existant component!
			assert(_entityToIndex.find(entity) != _entityToIndex.end());

			// copy last element over entity to-be-removed
			size_t indexOfRemovedEntity = _entityToIndex[entity];
			size_t indexOfLastElement = _size - 1;
			_components[indexOfRemovedEntity] = _components[indexOfLastElement];

			// update index mapping
			Entity entityOfLastElement = _indexToEntity[indexOfLastElement];
			_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
			_indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

			// erase the component
			_entityToIndex.erase(entity);
			_indexToEntity.erase(indexOfLastElement);

			_size--;
		}

		Component& GetComponent(Entity entity)
		{
			// Component doesn't exist!
			assert(_entityToIndex.find(entity) != _entityToIndex.end());

			return _components[_entityToIndex[entity]];
		}

		void EntityDestroyed(Entity entity) override
		{
			if (_entityToIndex.find(entity) != _entityToIndex.end())
			{
				RemoveComponent(entity);
			}
		}

	private:
		std::array<Component, MAX_ENTITIES>		_components;
		std::unordered_map<Entity, size_t>		_entityToIndex;
		std::unordered_map<size_t, Entity>		_indexToEntity;
		size_t									_size;
	};
}
