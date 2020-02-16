#pragma once
#include "Core/IvyPCH.h"
#include "Event/Event.h"
#include "World/World.h"
#include "Window/Window.h"


int main(int argc, char* argv[]);

namespace Ivy
{
	class IVY_API Application
	{
	public:
		Application						();
		virtual ~Application			();
		
		virtual void Tick				() = 0;
		void RegisterEventCallback		(EventCategory category, EventCallback callback);
		void UnregisterEventCallback	(EventCategory category, EventCallback callback);

	private:
		int Init						();
		int Run							(int argc, char* argv[]);

		Ivy::Ref<_Ivy::Window>			_window;
		Ivy::Ref<_Ivy::World>			_world;

		friend int ::main(int argc, char* argv[]);
	};
}


