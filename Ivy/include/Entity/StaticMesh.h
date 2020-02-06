#pragma once
#include "Core/IvyPCH.h"
#include "Entity/Entity.h"

namespace Ivy
{
	class IVY_API StaticMesh : public Entity
	{
	public:
		static Ivy::Ref<StaticMesh> Create(
			std::string sourcePath);

	public:
		const std::string& GetMeshPath() const { return _sourcePath; }
		~StaticMesh() {}

	protected:
		StaticMesh() {};

	private:
		std::string _sourcePath;
	};
}