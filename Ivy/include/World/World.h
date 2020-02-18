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
		static Ivy::Ref<World> Create();

	public:
		void Update(Ivy::Ref<Window> window);
		~World() {};

	private:
		World() {};
		Ivy::Ref<ECS> _ecs;
	};
}