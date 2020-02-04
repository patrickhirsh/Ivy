#pragma once
#include "Core/IvyPCH.h"
#include "Window/Window.h"

namespace Ivy
{
	class IvyEngine
	{
	public:
		static bool Run(int argc, char* argv[]);

	private:
		static Window*			_IvyWindow;
		static GLFWwindow*		_GLFWWindow;

		static bool initEngine();
		static bool initOpenGL();
		static bool initWindow();
		static bool initInput();
	};
}