#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"

namespace _Ivy
{

	class System
	{
	public:
		virtual void Update		(Ivy::Ref<Window> window) = 0;
		virtual ~System			() {};
		std::set<Entity>		Entities;
	};
}