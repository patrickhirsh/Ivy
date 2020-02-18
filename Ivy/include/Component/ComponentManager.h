#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"
#include "Component/Component.h"
#include "Component/ComponentArray.h"

namespace _Ivy
{
	class ComponentManager
	{
	public:
		template<typename Component>
		void RegisterComponent()
		{
			const char* typeName = typeid(Component).name();

			// Registering an already registered component!
			assert(_componentTypes.find(typeName) == _componentTypes.end());

			_componentTypes.insert({ typeName, _nextComponentType });
			_componentArrays.insert({ typeName, Ivy::Ref<ComponentArray<Component>>(new ComponentArray<Component>()) });
			_nextComponentType++;
		}

		template<typename Component>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(Component).name();

			// Getting a non-existant component type! (did you register it?)
			assert(_componentTypes.find(typeName) != _componentTypes.end());

			return _componentTypes[typeName];
		}

		template<typename Component>
		void AddComponent(Entity entity, Component component)
		{
			GetComponentArray<Component>()->AddComponent(entity, component);
		}

		template<typename Component>
		void RemoveComponent(Entity entity)
		{
			GetComponentArray<Component>()->RemoveComponent(entity);
		}

		template<typename Component>
		Component& GetComponent(Entity entity)
		{
			return GetComponentArray<Component>()->GetComponent(entity);
		}

		void EntityDestroyed(Entity entity)
		{
			for (auto const& component : _componentArrays)
			{
				component.second->EntityDestroyed(entity);
			}
		}

	private:
		std::unordered_map<const char*, ComponentType> _componentTypes{};
		std::unordered_map<const char*, Ivy::Ref<IComponentArray>> _componentArrays{};
		ComponentType _nextComponentType{};

		template<typename Component>
		Ivy::Ref<ComponentArray<Component>> GetComponentArray()
		{
			const char* typeName = typeid(Component).name();

			// Component not registered before use!
			assert(_componentTypes.find(typeName) != _componentTypes.end());

			return std::static_pointer_cast<ComponentArray<Component>>(_componentArrays[typeName]);
		}
	};


}
