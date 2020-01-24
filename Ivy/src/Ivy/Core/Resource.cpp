#include "IvyPCH.h"
#include "Resource.h"

namespace _Ivy
{
	std::unordered_map<std::string, Ivy::Ref<cy::TriMesh>> Resource::_objResources;

	void Resource::AddOBJResource(std::string objPath)
	{
		auto resource = Ivy::CreateRef<cy::TriMesh>();
		if (!resource->LoadFromFileObj(objPath.c_str())) { /* Error.. */ }
		_objResources.emplace(objPath, resource);
	}

	Ivy::Ref<cy::TriMesh> Resource::GetOBJResource(std::string objPath)
	{
		Ivy::Ref<cy::TriMesh> resource = nullptr;
		try { resource = _objResources.at(objPath); }
		catch (char const* e)
		{
			/* Warn.. */
			AddOBJResource(objPath);
			resource = _objResources.at(objPath);
		}
		return resource;
	}
}