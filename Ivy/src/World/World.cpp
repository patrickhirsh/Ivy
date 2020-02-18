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

		// register render system
		world->_ecs->RegisterSystem<Renderer>();
		ComponentSignature signature;
		signature.set(world->_ecs->GetComponentType<Ivy::Transform>());
		signature.set(world->_ecs->GetComponentType<Ivy::Mesh>());
		world->_ecs->SetSystemSignature<Renderer>(signature);

		auto transform = Ivy::Transform();
		transform.Position.Z = -50;
		auto mesh = Ivy::Mesh();
		mesh.SourceMeshPath = "teapot.obj";
		mesh.SourceTexturePath = "brick.png";
		Entity teapot = world->_ecs->CreateEntity();
		world->_ecs->AddComponent<Ivy::Transform>(teapot, transform);
		world->_ecs->AddComponent<Ivy::Mesh>(teapot, mesh);

		return world;
	}

	void World::Update(Ivy::Ref<Window> window)
	{
		_ecs->UpdateSystems(window, _ecs);
	}
}