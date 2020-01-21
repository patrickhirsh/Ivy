#pragma once
#include "GLFW/glfw3.h"
#include "Core.h"

namespace Ivy
{
	extern bool _exitFlag;
	extern void keyEventHandle(GLFWwindow* window, int key, int scancode, int action, int mods);

	class IVY_API Window
	{
	public:
		Window();
		~Window();
	};
}