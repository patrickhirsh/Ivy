#pragma once
#include "Core/IvyPCH.h"

namespace _Ivy
{
	class Render;
}

namespace Ivy
{
	class IVY_API Object
	{
	public:
		static Ivy::Ref<Object> Create(
			std::string						objPath);

	public:
		~Object								() {};
	private:
		Object								() {};
		std::string							_objPath;

		friend class _Ivy::Render;
	};
}
