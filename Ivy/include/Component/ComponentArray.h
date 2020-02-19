#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"

namespace _Ivy
{
	class IComponentArray
	{
	public:
		virtual ~IComponentArray				() {};
		virtual void EntityDestroyed			(Ivy::Entity entity) = 0;
	};

	template<typename Component>
	class ComponentArray : public IComponentArray
	{
	public:
		void AddComponent(Ivy::Entity entity, Component component)
		{
			if (_entityToIndex.find(entity) == _entityToIndex.end())
			{
				size_t newIndex = _size;
				_entityToIndex[entity] = newIndex;
				_indexToEntity[newIndex] = entity;
				_components[newIndex] = component;
				_size++;
			}
			else
			{
				ASSERTM(false, "Tried to add duplicate component " << typeid(Component).name() 
					<< " to entity " << entity << "!");
			}
		}

		void RemoveComponent(Ivy::Entity entity)
		{
			if (_entityToIndex.find(entity) != _entityToIndex.end())
			{
				// copy last element over entity to-be-removed
				size_t indexOfRemovedEntity = _entityToIndex[entity];
				size_t indexOfLastElement = _size - 1;
				_components[indexOfRemovedEntity] = _components[indexOfLastElement];

				// update index mapping
				Ivy::Entity entityOfLastElement = _indexToEntity[indexOfLastElement];
				_entityToIndex[entityOfLastElement] = indexOfRemovedEntity;
				_indexToEntity[indexOfRemovedEntity] = entityOfLastElement;

				// erase the component
				_entityToIndex.erase(entity);
				_indexToEntity.erase(indexOfLastElement);

				_size--;
			}
			else
			{
				ASSERTM(false, "Tried to remove non-existant component " << typeid(Component).name() 
					<< " from entity " << entity << "!");
			}
		}

		bool HasComponent(Ivy::Entity entity)
		{
			return _entityToIndex.find(entity) != _entityToIndex.end();
		}

		Component& GetComponent(Ivy::Entity entity)
		{
			ASSERTMF(_entityToIndex.find(entity) != _entityToIndex.end(), 
				"Entity " << entity << " doesn't have a " << typeid(Component).name() << " component!");
			
			return _components[_entityToIndex[entity]];
		}

		void EntityDestroyed(Ivy::Entity entity) override
		{
			if (_entityToIndex.find(entity) != _entityToIndex.end())
			{
				RemoveComponent(entity);
			}
		}

	private:
		std::array<Component, MAX_ENTITIES>		_components;
		std::unordered_map<Ivy::Entity, size_t>	_entityToIndex;
		std::unordered_map<size_t, Ivy::Entity>	_indexToEntity;
		size_t									_size;
	};
}
