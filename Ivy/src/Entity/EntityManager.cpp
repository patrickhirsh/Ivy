#include "Core/IvyPCH.h"
#include "Entity/EntityManager.h"

namespace _Ivy
{
	EntityManager::EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			_availableEntities.push(entity);
		}
	}

	Entity EntityManager::CreateEntity()
	{
		// too many entities created!
		assert(_livingEntityCount < MAX_ENTITIES);

		Entity id = _availableEntities.front();
		_availableEntities.pop();
		_livingEntityCount++;

		return id;
	}

	void EntityManager::DestroyEntity(Entity entity)
	{
		// entity out of range!
		assert(entity < MAX_ENTITIES);

		// reset bit signature to all 0's
		_componentSignatures[entity].reset();

		_availableEntities.push(entity);
		_livingEntityCount--;
	}

	void EntityManager::SetSignature(Entity entity, ComponentSignature signature)
	{
		// entity out of range!
		assert(entity < MAX_ENTITIES);

		_componentSignatures[entity] = signature;
	}

	ComponentSignature EntityManager::GetSignature(Entity entity)
	{
		// entity out of range!
		assert(entity < MAX_ENTITIES);

		return _componentSignatures[entity];
	}
}