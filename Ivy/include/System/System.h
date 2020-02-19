#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"

namespace _Ivy
{
	class ECS;
	class Window;

	class System
	{
	public:
		virtual void Update		(Ivy::Ref<Window> window, Ivy::Ref<ECS> ecs) = 0;
		virtual ~System			() {};
		std::set<Ivy::Entity>		Entities;
	};
}