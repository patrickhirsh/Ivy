#pragma once
#include "Core/IvyPCH.h"
#include "Object/Object.h"

namespace Ivy
{
	class StaticMesh : public Object
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