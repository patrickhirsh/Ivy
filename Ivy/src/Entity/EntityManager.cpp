#include "Core/IvyPCH.h"
#include "Entity/EntityManager.h"

namespace _Ivy
{
	EntityManager::EntityManager()
	{
		for (Ivy::Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			_availableEntities.push(entity);
		}
	}

	Ivy::Entity EntityManager::CreateEntity()
	{
		ASSERTMF(_livingEntityCount < MAX_ENTITIES, "Exceeded entity limit!");
		Ivy::Entity id = _availableEntities.front();
		_availableEntities.pop();
		_livingEntityCount++;
		return id;
	}

	void EntityManager::DestroyEntity(Ivy::Entity entity)
	{
		ASSERTMF(entity < MAX_ENTITIES, "Entity out of range!");
		_componentSignatures[entity].reset();
		_availableEntities.push(entity);
		_livingEntityCount--;
	}

	void EntityManager::SetSignature(Ivy::Entity entity, ComponentSignature signature)
	{
		ASSERTMF(entity < MAX_ENTITIES, "Entity out of range!");
		_componentSignatures[entity] = signature;
	}

	ComponentSignature EntityManager::GetSignature(Ivy::Entity entity)
	{
		ASSERTMF(entity < MAX_ENTITIES, "Entity out of range!");
		return _componentSignatures[entity];
	}
}