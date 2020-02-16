#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"
#include "Component/Component.h"

namespace _Ivy
{
	class EntityManager
	{
	public:
		EntityManager						();
		~EntityManager						() {};
		Entity CreateEntity					();
		void DestroyEntity					(Entity entity);
		void SetSignature					(Entity entity, ComponentSignature signature);
		ComponentSignature GetSignature		(Entity entity);

	private:
		std::queue<Entity> _availableEntities{};
		std::array<ComponentSignature, MAX_ENTITIES> _componentSignatures{};
		unsigned int _livingEntityCount{};
	};
}