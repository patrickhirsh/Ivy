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

			ASSERTMF(_componentTypes.find(typeName) == _componentTypes.end(), 
				"Tried to register component " << typeName << ", which has already been registered!");
			
			_componentTypes.insert({ typeName, _nextComponentType });
			_componentArrays.insert({ typeName, Ivy::Ref<ComponentArray<Component>>(new ComponentArray<Component>()) });
			_nextComponentType++;
		}

		template<typename Component>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(Component).name();
			ASSERTMF(_componentTypes.find(typeName) != _componentTypes.end(),
				"Tried to get unregistered component type " << typeName << "!");

			return _componentTypes[typeName];
		}

		template<typename Component>
		void AddComponent(Ivy::Entity entity, Component component)
		{
			GetComponentArray<Component>()->AddComponent(entity, component);
		}

		template<typename Component>
		void RemoveComponent(Ivy::Entity entity)
		{
			GetComponentArray<Component>()->RemoveComponent(entity);
		}

		template<typename Component>
		bool HasComponent(Ivy::Entity entity)
		{
			return GetComponentArray<Component>()->HasComponent(entity);
		}

		template<typename Component>
		Component& GetComponent(Ivy::Entity entity)
		{
			return GetComponentArray<Component>()->GetComponent(entity);
		}

		void EntityDestroyed(Ivy::Entity entity)
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
			ASSERTMF(_componentTypes.find(typeName) != _componentTypes.end(),
				"Tried to get component array for unregistered component " << typeName << "!");

			return std::static_pointer_cast<ComponentArray<Component>>(_componentArrays[typeName]);
		}
	};


}
