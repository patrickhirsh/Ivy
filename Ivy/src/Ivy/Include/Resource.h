#pragma once
#include "IvyPCH.h"

namespace _Ivy
{
	class Resource
	{
	public:
		static void AddOBJResource(std::string objPath);
		static Ivy::Ref<cy::TriMesh> GetOBJResource(std::string objPath);

	private:
		static std::unordered_map<std::string, Ivy::Ref<cy::TriMesh>> _objResources;
	};
}