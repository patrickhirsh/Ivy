#pragma once
#include "Core/IvyPCH.h"
#include "Render/Render.h"
#include "Event/Event.h"


int main(int argc, char* argv[]);

namespace Ivy
{
	class IVY_API Application
	{
	public:
		Application();
		virtual ~Application();
	
	private:
		bool Run(int argc, char* argv[]);

		GLFWwindow* _window;
		_Ivy::Render* _render;
		_Ivy::EventDispatcher _event;

		bool initEngine();
		bool initOpenGL();

		friend int ::main(int argc, char* argv[]);
	};
}


