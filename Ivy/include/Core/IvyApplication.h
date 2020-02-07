#pragma once
#include "Core/IvyPCH.h"

namespace Ivy
{
	class IVY_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
}
