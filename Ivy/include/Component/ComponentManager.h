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
		void RegisterComponent();

		template<typename Component>
		ComponentType GetComponentType();

		template<typename Component>
		void AddComponent(Entity entity, Component component);

		template<typename Component>
		void RemoveComponent(Entity entity);

		template<typename Component>
		Component& GetComponent(Entity entity);

		void EntityDestroyed(Entity entity);

	private:
		std::unordered_map<const char*, ComponentType> _componentTypes{};
		std::unordered_map<const char*, Ivy::Ref<IComponentArray>> _componentArrays{};
		ComponentType _nextComponentType{};

		template<typename Component>
		Ivy::Ref<ComponentArray<Component>> GetComponentArray();
	};


}
