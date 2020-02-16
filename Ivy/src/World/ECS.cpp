#include "Core/IvyPCH.h"
#include "World/ECS.h"

// REF: https://austinmorlan.com/posts/entity_component_system/

/* This is a simple ECS implementation intended to get the engine working. 
Eventually, we'll need to modify (or, potentially, entirely rework) the way
this system works. Ideally - we'll implement an archetype-based system. */

namespace _Ivy
{
	Ivy::Ref<ECS> ECS::Create()
	{
		auto ecs = Ivy::Ref<ECS>(new ECS());
		ecs->_systemManager = Ivy::Ref<SystemManager>(new SystemManager());
		ecs->_entityManager = Ivy::Ref<EntityManager>(new EntityManager());
		ecs->_componentManager = Ivy::Ref<ComponentManager>(new ComponentManager());
		return ecs;
	}

	Entity ECS::CreateEntity()
	{
		return _entityManager->CreateEntity();
	}

	void ECS::DestroyEntity(Entity entity)
	{
		_entityManager->DestroyEntity(entity);
	}

	template<typename Component>
	void ECS::RegisterComponent()
	{
		_componentManager->RegisterComponent<Component>();
	}

	template<typename Component>
	void ECS::AddComponent(Entity entity, Component component)
	{
		_componentManager->AddComponent<Component>(entity, component);

		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<Component>(), true);
		_entityManager->SetSignature(entity, signature);
		_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename Component>
	void ECS::RemoveComponent(Entity entity)
	{
		_componentManager->RemoveComponent<Component>(entity);

		auto signature = _entityManager->GetSignature(entity);
		signature.set(_componentManager->GetComponentType<Component>(), false);
		_entityManager->SetSignature(entity, signature);

		_systemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename Component>
	Component& ECS::GetComponent(Entity entity)
	{
		return _componentManager->GetComponent<Component>(entity);
	}

	template<typename Component>
	ComponentType ECS::GetComponentType()
	{
		return _componentManager->GetComponentType<Component>();
	}

	template<typename System>
	Ivy::Ref<System> ECS::RegisterSystem()
	{
		return _systemManager->RegisterSystem<System>();
	}

	template<typename System>
	void ECS::SetSystemSignature(ComponentSignature signature)
	{
		_systemManager->SetSignature<System>(signature);
	}

	void ECS::UpdateSystems(Ivy::Ref<Window> window)
	{
		_systemManager->UpdateSystems(window);
	}
}