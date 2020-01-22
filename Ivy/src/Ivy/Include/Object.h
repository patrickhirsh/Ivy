#pragma once
#include "IvyPCH.h"

namespace Ivy
{
	class Object
	{
	public:
		static Ivy::Ref<Object> Create(
			std::string						objPath);

	public:
		~Object								();
	private:
		Object								();
		std::string							_objPath;
	};
}
