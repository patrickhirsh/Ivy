#include "Core/IvyPCH.h"
#include "Component/ComponentArray.h"

namespace _Ivy
{
	template<typename Component>
	void ComponentArray<Component>::AddComponent(Entity entity, Component component)
	{
		// Entity with duplicate components!
		assert(_entityToIndex.find(entity) == _entityToIndex.end());

		size_t newIndex = _size;
		_entityToIndex[entity] = newIndex;
		_indexToEntity[newIndex] = entity;
		_components[newIndex] = component;
		_size++;
	}

	template<typename Component>
	void ComponentArray<Component>::RemoveComponent(Entity entity)
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

	template<typename Component>
	Component& ComponentArray<Component>::GetComponent(Entity entity)
	{
		// Component doesn't exist!
		assert(_entityToIndex.find(entity) != _entityToIndex.end());

		return _components[_entityToIndex[entity]];
	}

	template<typename Component>
	void ComponentArray<Component>::EntityDestroyed(Entity entity)
	{
		if (_entityToIndex.find(entity) != _entityToIndex.end())
		{
			RemoveComponent(entity);
		}
	}
}