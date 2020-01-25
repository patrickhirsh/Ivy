#include "IvyPCH.h"
#include "Resource.h"

namespace _Ivy
{
	std::unordered_map<std::string, Ivy::Ref<cy::TriMesh>> Resource::_objResources;


	std::string Resource::LoadShader(std::string shaderPath)
	{
		std::string fullPath = _Ivy::GetResourceDirectory() + shaderPath;
		std::ifstream file(shaderPath);
		if (!file) { /* ERROR.. */ return ""; }

		std::stringstream stream;
		stream << file.rdbuf();
		file.close();

		return stream.str();
	}

	void Resource::AddOBJResource(std::string objPath)
	{
		std::string fullPath = _Ivy::GetResourceDirectory() + objPath;
		auto resource = Ivy::CreateRef<cy::TriMesh>();
		if (!resource->LoadFromFileObj(fullPath.c_str())) { LOG_ERROR("failed to load OBJ file from path: " << fullPath); }
		_objResources.emplace(objPath, resource);
	}

	Ivy::Ref<cy::TriMesh> Resource::GetOBJResource(std::string objPath)
	{
		Ivy::Ref<cy::TriMesh> resource = nullptr;
		try { resource = _objResources.at(objPath); }
		catch (char const* e)
		{
			LOG_WARN("performing forced load of OBJ: " << objPath);
			AddOBJResource(objPath);
			resource = _objResources.at(objPath);
		}
		return resource;
	}
}