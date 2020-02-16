#pragma once
#include "Core/IvyPCH.h"
#include "System/System.h"
#include "Entity/Entity.h"
#include "Component/Component.h"

namespace _Ivy
{
	class Window;

	class SystemManager
	{
	public:
		template<typename System>
		Ivy::Ref<System> RegisterSystem();

		template<typename System>
		void SetSignature(ComponentSignature signature);

		void EntityDestroyed(Entity entity);
		void EntitySignatureChanged(Entity entity, ComponentSignature signature);

		void UpdateSystems(Ivy::Ref<Window> window);

	private:
		std::unordered_map<const char*, ComponentSignature> _signatures;
		std::unordered_map<const char*, Ivy::Ref<System>> _systems;
	};
}