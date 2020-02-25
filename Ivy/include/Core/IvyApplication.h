#pragma once
#include "Core/IvyPCH.h"
#include "Event/Event.h"
#include "World/World.h"
#include "Window/Window.h"
#include "Entity/Entity.h"
#include "Component/Component.h"

#define IMPL_EXPL_COMP_TEMPLATE_INSTANTIATION(c) \
template IVY_API void Application::AddComponent<c>(Entity entity, c component); \
template IVY_API void Application::RemoveComponent<c>(Entity entity); \
template IVY_API bool Application::HasComponent<c>(Entity entity); \
template IVY_API c& Application::GetComponent<c>(Entity entity); \

int main(int argc, char* argv[]);

namespace Ivy
{
	class IVY_API Application
	{
	public:
		Application						();
		virtual ~Application			();
		
		virtual void Tick				() = 0;
		void RegisterEventCallback		(EventCategory category, EventCallback callback);
		void UnregisterEventCallback	(EventCategory category, EventCallback callback);

		/* Entity Methods */
		Entity CreateEntity				() { return _world->CreateEntity(); }
		void DestroyEntity				(Entity entity) { _world->DestroyEntity(entity); }

		/* Component Methods*/
		
		template<typename Component>
		void AddComponent				(Entity entity, Component component) { _world->AddComponent<Component>(entity, component); }
		template<typename Component>
		void RemoveComponent			(Entity entity) { _world->RemoveComponent<Component>(entity); }
		template<typename Component>
		bool HasComponent				(Entity entity) { return _world->HasComponent<Component>(entity); }
		template<typename Component>
		Component& GetComponent			(Entity entity) { return _world->GetComponent<Component>(entity); }

		
	private:
		int Init						();
		int Run							(int argc, char* argv[]);

		Ivy::Ref<_Ivy::Window>			_window;
		Ivy::Ref<_Ivy::World>			_world;

		friend int ::main(int argc, char* argv[]);
	};

	IMPL_EXPL_COMP_TEMPLATE_INSTANTIATION(Transform)
	IMPL_EXPL_COMP_TEMPLATE_INSTANTIATION(Mesh)
	IMPL_EXPL_COMP_TEMPLATE_INSTANTIATION(Material)
	IMPL_EXPL_COMP_TEMPLATE_INSTANTIATION(Cubemap)
}


