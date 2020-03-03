#include "Core/IvyPCH.h"
#include "Core/IvyApplication.h"

namespace Ivy
{
	Application::Application()
	{
		int status = Init();
		if (status != IVY_OK) { exit(status); }
	}

	Application::~Application()
	{
		glfwTerminate();
	}

	void Application::RegisterEventCallback(EventCategory category, EventCallback callback)
	{
		_Ivy::EventDispatcher::Register(category, callback);
	}

	void Application::UnregisterEventCallback(EventCategory category, EventCallback callback)
	{
		_Ivy::EventDispatcher::Unregister(category, callback);
	}

	int Application::Init()
	{
		// init GLFW
		if (!glfwInit())
		{
			LOG_ERROR("GLFW failed to initialize!");
			return IVY_FATAL;
		}

		// create window
		_window = _Ivy::Window::Create(900, 900, "Ivy Engine");
		if (!_window)
		{
			LOG_ERROR("Failed to create Ivy window!");
			return IVY_FATAL;
		}

		// create world
		_world = _Ivy::World::Create();
		if (!_world)
		{
			LOG_ERROR("Failed to create Ivy world!");
			return IVY_FATAL;
		}

		LOG_SYSTEM_STARTUP();
		return IVY_OK;
	}

	int Application::Run(int argc, char* argv[])
	{
		while (_window->IsActive())
		{
			Tick();
			_window->BeginFrame();
			_world->Update(_window);
			_window->EndFrame();
		}
		return true;
	}
}