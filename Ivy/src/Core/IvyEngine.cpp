#include "Core/IvyPCH.h"
#include "Core/IvyEngine.h"

namespace Ivy
{
	Window* IvyEngine::_IvyWindow;
	GLFWwindow* IvyEngine::_GLFWWindow;

	bool IvyEngine::Run(int argc, char* argv[])
	{
		if (!initEngine()) { return false; }

		while (_IvyWindow->IsActive())
		{
			_IvyWindow->Update();
		}

		Shutdown();
		return true;
	}

	void IvyEngine::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		glfwTerminate();
	}

	bool IvyEngine::initEngine()
	{
		LOG_SYS("=============== Ivy Engine Startup ===============");
		if (!initOpenGL())	{ return false; }
		if (!initImGui())	{ return false; }
		if (!initWindow())	{ return false; }
		if (!initInput())	{ return false; }
		LOG_SYS("==================================================");
		return true;
	}

	bool IvyEngine::initOpenGL()
	{
		LOG_SYS("  <OpenGL>");

		if (!glfwInit()) 
		{
			LOG_ERROR("    GLFW failed to initialize!");
			return false; 
		}

		_GLFWWindow = glfwCreateWindow(900, 900, "Ivy Engine", NULL, NULL);
		if (!_GLFWWindow) 
		{
			LOG_ERROR("    GLFW falied to create a new window!");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(_GLFWWindow);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwSwapInterval(1);

		if (glewInit() != GLEW_OK) 
		{
			LOG_ERROR("    GLEW failed to initialize!");
			glfwTerminate();
			return false;
		}

		LOG_SYS("    Hardware Vendor:    " << (glGetString(GL_VENDOR) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VENDOR)));
		LOG_SYS("    Hardware Renderer:  " << (glGetString(GL_RENDERER) == GL_NONE ? "N/A" : (const char*)glGetString(GL_RENDERER)));
		LOG_SYS("    OpenGL Version:     " << (glGetString(GL_VERSION) == GL_NONE ? "N/A" : (const char*)glGetString(GL_VERSION)));
		return true;
	}

	bool IvyEngine::initImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(_GLFWWindow, true);
		ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
		return true;
	}

	bool IvyEngine::initWindow()
	{
		LOG_SYS("  <Window>");

		_IvyWindow = new Window(_GLFWWindow);
		return true;
	}

	bool IvyEngine::initInput()
	{
		return true;
	}
}