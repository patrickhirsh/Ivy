#pragma once
#include "Core/IvyPCH.h"
#include "System/System.h"
#include "Entity/Entity.h"
#include "Component/Component.h"

namespace _Ivy
{
	class ECS;
	class Window;

	class SystemManager
	{
	public:
		template<typename System>
		Ivy::Ref<System> RegisterSystem()
		{
			const char* typeName = typeid(System).name();

			// Registering a system that's already registered!
			assert(_systems.find(typeName) == _systems.end());

			auto system = Ivy::Ref<System>(new System());
			_systems.insert({ typeName, system });
			return system;
		}

		template<typename System>
		void SetSignature(ComponentSignature signature)
		{
			const char* typeName = typeid(System).name();

			// System isn't registered!
			assert(_systems.find(typeName) != _systems.end());

			_signatures.insert({ typeName, signature });
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& system : _systems)
			{
				system.second->Entities.erase(entity);
			}
		}

		void EntitySignatureChanged(Entity entity, ComponentSignature signature)
		{
			for (auto const& pair : _systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& systemSignature = _signatures[type];

				// Entity signature matches system signature - insert into set
				if ((signature & systemSignature) == systemSignature)
				{
					system->Entities.insert(entity);
				}

				// Entity signature does not match - erase
				else
				{
					system->Entities.erase(entity);
				}
			}
		}

		void UpdateSystems(Ivy::Ref<Window> window, Ivy::Ref<ECS> ecs)
		{
			for (auto system : _systems)
			{
				system.second->Update(window, ecs);
			}
		}

	private:
		std::unordered_map<const char*, ComponentSignature> _signatures;
		std::unordered_map<const char*, Ivy::Ref<System>> _systems;
	};
}