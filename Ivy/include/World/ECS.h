#pragma once
#include "Core/IvyPCH.h"
#include "System/SystemManager.h"
#include "Entity/EntityManager.h"
#include "Component/ComponentManager.h"

namespace _Ivy
{
	class ECS
	{
	public:
		static Ivy::Ref<ECS> Create();

	public:
		~ECS() {};


		/* Entity Methods */

		Entity CreateEntity();
		void DestroyEntity(Entity entity);


		/* Component Methods */

		template<typename Component>
		void RegisterComponent();

		template<typename Component>
		void AddComponent(Entity entity, Component component);

		template<typename Component>
		void RemoveComponent(Entity entity);

		template<typename Component>
		Component& GetComponent(Entity entity);

		template<typename Component>
		ComponentType GetComponentType();


		/* System Methods */

		template<typename System>
		Ivy::Ref<System> RegisterSystem();

		template<typename System>
		void SetSystemSignature(ComponentSignature signature);

		void UpdateSystems(Ivy::Ref<Window> window);

	private:
		ECS() {};
		Ivy::Ref<SystemManager>			_systemManager;
		Ivy::Ref<EntityManager>			_entityManager;
		Ivy::Ref<ComponentManager>		_componentManager;
	};
}