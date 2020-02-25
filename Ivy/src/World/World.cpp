#include "Core/IvyPCH.h"
#include "World/World.h"

namespace _Ivy
{
	Ivy::Ref<World> World::Create()
	{
		auto world = Ivy::Ref<World>(new World());

		// register components
		world->_ecs = ECS::Create();
		world->_ecs->RegisterComponent<Ivy::Transform>();
		world->_ecs->RegisterComponent<Ivy::Mesh>();
		world->_ecs->RegisterComponent<Ivy::Material>();
		world->_ecs->RegisterComponent<Ivy::Cubemap>();

		// register render system
		world->_renderer = world->_ecs->RegisterSystem<Renderer>();
		ComponentSignature signature;
		signature.set(world->_ecs->GetComponentType<Ivy::Transform>());
		signature.set(world->_ecs->GetComponentType<Ivy::Mesh>());
		world->_ecs->SetSystemSignature<Renderer>(signature);

		return world;
	}

	void World::Update(Ivy::Ref<Window> window)
	{
		_ecs->UpdateSystems(window, _ecs);
	}
}