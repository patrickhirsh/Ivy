#include "Core/IvyPCH.h"
#include "Component/ComponentManager.h"

namespace _Ivy
{
	template<typename Component>
	void ComponentManager::RegisterComponent()
	{
		const char* typeName = typeid(Component).name();

		// Registering an already registered component!
		assert(_componentTypes.find(typeName) == _componentTypes.end());

		_componentTypes.insert({ typeName, _nextComponentType });
		_componentArrays.insert({ typeName, Ivy::Ref<ComponentArray<Component>>(new ComponentArray<Component>()) });
		_nextComponentType++;
	}

	template<typename Component>
	ComponentType ComponentManager::GetComponentType()
	{
		const char* typeName = typeid(Component).name();

		// Getting a non-existant component type! (did you register it?)
		assert(_componentTypes.find(typeName) != _componentTypes.end());

		return _componentTypes[typeName];
	}

	template<typename Component>
	void ComponentManager::AddComponent(Entity entity, Component component)
	{
		GetComponentArray<Component>()->AddComponent(entity, component);
	}

	template<typename Component>
	void ComponentManager::RemoveComponent(Entity entity)
	{
		GetComponentArray<Component>()->RemoveComponent(entity);
	}

	template<typename Component>
	Component& ComponentManager::GetComponent(Entity entity)
	{
		return GetComponentArray<Component>()->GetComponent(entity);
	}

	void ComponentManager::EntityDestroyed(Entity entity)
	{
		for (auto const& component : _componentArrays)
		{
			component.second->EntityDestroyed(entity);
		}
	}
}