#pragma once
#include "Core/IvyPCH.h"
#include "Render/Renderer.h"
#include "Event/Event.h"


int main(int argc, char* argv[]);

namespace Ivy
{
	class IVY_API Application
	{
	public:
		Application						();
		virtual ~Application			();
		void RegisterEventCallback		(Ivy::EventCategory category, Ivy::EventCallback callback);
		void UnregisterEventCallback	(Ivy::EventCategory category, Ivy::EventCallback callback);
	
	private:
		bool Run						(int argc, char* argv[]);

		bool							_active;
		GLFWwindow*						_window;
		_Ivy::Renderer*					_renderer;

		bool initEngine					();
		bool initGLFW					();
		bool initGLEW					();
		bool initRenderer				();

		void obtainWindowContext		();

		friend int ::main(int argc, char* argv[]);
	};
}


