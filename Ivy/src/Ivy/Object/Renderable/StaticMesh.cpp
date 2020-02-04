#include "Core/IvyPCH.h"
#include "Object/Renderable/StaticMesh.h"

namespace Ivy
{
	Ivy::Ref<StaticMesh> StaticMesh::Create(
		std::string sourcePath)
	{
		Ivy::Ref<StaticMesh> object = Ivy::Ref<StaticMesh>(new StaticMesh());
		object->_sourcePath = sourcePath;
		return object;
	}
}