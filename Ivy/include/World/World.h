#pragma once
#include "Core/IvyPCH.h"
#include "World/ECS.h"
#include "Window/Window.h"
#include "Component/Component.h"
#include "System/Renderer.h"


namespace _Ivy
{
	class World
	{
	public:
		static Ivy::Ref<World> Create	();

	public:
		void Update						(Ivy::Ref<Window> window);
		~World							() {};

		/* Entity Methods */
		Ivy::Entity CreateEntity		() { return _ecs->CreateEntity(); }
		void DestroyEntity				(Ivy::Entity entity) { _ecs->DestroyEntity(entity); }

		/* Component Methods */
		template<typename Component>
		void AddComponent				(Ivy::Entity entity, Component component) { _ecs->AddComponent<Component>(entity, component); }
		template<typename Component>
		void RemoveComponent			(Ivy::Entity entity) { _ecs->RemoveComponent<Component>(entity); }
		template<typename Component>
		bool HasComponent				(Ivy::Entity entity) { return _ecs->HasComponent<Component>(entity); }
		template<typename Component>
		Component& GetComponent			(Ivy::Entity entity) { return _ecs->GetComponent<Component>(entity); }

	private:
		World() {};
		Ivy::Ref<ECS>					_ecs;

		// Systems
		Ivy::Ref<Renderer>				_renderer;
	};
}