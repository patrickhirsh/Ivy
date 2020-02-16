#include "Core/IvyPCH.h"
#include "System/SystemManager.h"

namespace _Ivy
{
	template<typename System>
	Ivy::Ref<System> SystemManager::RegisterSystem()
	{
		const char* typeName = typeid(System).name();

		// Registering a system that's already registered!
		assert(_systems.find(typeName) == _systems.end());

		auto system = Ivy::Ref<System>(new System());
		_systems.insert({ typeName, system });
		return system;
	}


	template<typename System>
	void SystemManager::SetSignature(ComponentSignature signature)
	{
		const char* typeName = typeid(System).name();

		// System isn't registered!
		assert(_systems.find(typeName) == _systems.end());

		_signatures.insert({ typeName, signature });
	}

	void SystemManager::EntityDestroyed(Entity entity)
	{
		for (auto const& system : _systems)
		{
			system.second->Entities.erase(entity);
		}
	}

	void SystemManager::EntitySignatureChanged(Entity entity, ComponentSignature signature)
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

	void SystemManager::UpdateSystems(Ivy::Ref<Window> window)
	{
		for (auto system : _systems)
		{
			system.second->Update(window);
		}
	}
}