#pragma once
#include "Core/IvyPCH.h"
#include "System/SystemManager.h"
#include "Entity/EntityManager.h"
#include "Component/ComponentManager.h"

// REF: https://austinmorlan.com/posts/entity_component_system/

/* This is a simple ECS implementation intended to get the engine working.
Eventually, we'll need to modify (or, potentially, entirely rework) the way
this system works. Ideally - we'll implement an archetype-based system. */

namespace _Ivy
{
	class Window;

	class ECS
	{
	public:
		static Ivy::Ref<ECS> Create() 
		{
			auto ecs = Ivy::Ref<ECS>(new ECS());
			ecs->_systemManager = Ivy::Ref<SystemManager>(new SystemManager());
			ecs->_entityManager = Ivy::Ref<EntityManager>(new EntityManager());
			ecs->_componentManager = Ivy::Ref<ComponentManager>(new ComponentManager());
			return ecs;
		}

	public:
		~ECS() {};


		/* Entity Methods */

		Ivy::Entity CreateEntity()
		{
			return _entityManager->CreateEntity();
		}

		void DestroyEntity(Ivy::Entity entity)
		{
			_entityManager->DestroyEntity(entity);
		}


		/* Component Methods */

		template<typename Component>
		void RegisterComponent()
		{
			_componentManager->RegisterComponent<Component>();
		}

		template<typename Component>
		void AddComponent(Ivy::Entity entity, Component component)
		{
			_componentManager->AddComponent<Component>(entity, component);

			auto signature = _entityManager->GetSignature(entity);
			signature.set(_componentManager->GetComponentType<Component>(), true);
			_entityManager->SetSignature(entity, signature);
			_systemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename Component>
		void RemoveComponent(Ivy::Entity entity)
		{
			_componentManager->RemoveComponent<Component>(entity);

			auto signature = _entityManager->GetSignature(entity);
			signature.set(_componentManager->GetComponentType<Component>(), false);
			_entityManager->SetSignature(entity, signature);

			_systemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename Component>
		bool HasComponent(Ivy::Entity entity)
		{
			return _componentManager->HasComponent<Component>(entity);
		}

		template<typename Component>
		Component& GetComponent(Ivy::Entity entity)
		{
			return _componentManager->GetComponent<Component>(entity);
		}

		template<typename Component>
		ComponentType GetComponentType()
		{
			return _componentManager->GetComponentType<Component>();
		}


		/* System Methods */

		template<typename System>
		Ivy::Ref<System> RegisterSystem()
		{
			return _systemManager->RegisterSystem<System>();
		}

		template<typename System>
		void SetSystemSignature(ComponentSignature signature)
		{
			_systemManager->SetSignature<System>(signature);
		}

		void UpdateSystems(Ivy::Ref<Window> window, Ivy::Ref<ECS> ecs)
		{
			_systemManager->UpdateSystems(window, ecs);
		}

	private:
		ECS() {};
		Ivy::Ref<SystemManager>			_systemManager;
		Ivy::Ref<EntityManager>			_entityManager;
		Ivy::Ref<ComponentManager>		_componentManager;
	};
}