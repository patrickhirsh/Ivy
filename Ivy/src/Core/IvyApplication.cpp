#include "Core/IvyPCH.h"
#include "Core/IvyApplication.h"

namespace Ivy
{
	Application::Application()
	{
		if (!initEngine()) { exit(false); }
	}

	Application::~Application()
	{

	}

	bool Application::Run(int argc, char* argv[])
	{
		while (true) {}
		return true;
	}

	bool Application::initEngine()
	{
		LOG_INIT_SYS_BEGIN

		if (!initOpenGL()) { return false; }

		LOG_INIT_SYS_END

		return true;
	}

	bool Application::initOpenGL()
	{
		LOG_INIT_SYS_HEADER("OpenGL");

		if (!glfwInit())
		{
			LOG_ERROR("GLFW failed to initialize!");
			return false;
		}

		_window = glfwCreateWindow(900, 900, "Ivy Engine", NULL, NULL);
		if (!_window)
		{
			LOG_ERROR("GLFW failed to create a new window!");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(_window);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwSwapInterval(1); VSYNC

		if (glewInit() != GLEW_OK)
		{
			LOG_ERROR("GLEW failed to initialize!");
			glfwTerminate();
			return false;
		}

		LOG_INIT_SYS_ENTRY("Hardware Vendor:    " << (glGetString(GL_VENDOR) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VENDOR)));
		LOG_INIT_SYS_ENTRY("Hardware Renderer:  " << (glGetString(GL_RENDERER) == GL_NONE ? "N/A" : (const char*)glGetString(GL_RENDERER)));
		LOG_INIT_SYS_ENTRY("OpenGL Version:     " << (glGetString(GL_VERSION) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VERSION)));
		return true;
	}
}