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
		Ivy::Entity CreateEntity			();
		void DestroyEntity					(Ivy::Entity entity);
		void SetSignature					(Ivy::Entity entity, ComponentSignature signature);
		ComponentSignature GetSignature		(Ivy::Entity entity);

	private:
		std::queue<Ivy::Entity> _availableEntities{};
		std::array<ComponentSignature, MAX_ENTITIES> _componentSignatures{};
		unsigned int _livingEntityCount{};
	};
}