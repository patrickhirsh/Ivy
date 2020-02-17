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
		glfwTerminate();
		delete _renderer;
	}

	void Application::RegisterEventCallback(EventCategory category, EventCallback callback)
	{
		_Ivy::EventDispatcher::Register(category, callback);
	}

	void Application::UnregisterEventCallback(EventCategory category, EventCallback callback)
	{
		_Ivy::EventDispatcher::Unregister(category, callback);
	}

	void Application::SetSceneTranslation(float transX, float transY, float transZ)
	{
		_renderer->SetSceneTranslation(cy::Vec3f(transX, transY, transZ));
	}

	void Application::SetSceneRotation(float angleX, float angleY, float angleZ)
	{
		_renderer->SetSceneRotation(angleX, angleY, angleZ);
	}

	bool Application::Run(int argc, char* argv[])
	{
		while (_active)
		{
			Tick();

			GLint width, height;
			obtainWindowContext();
			glfwGetWindowSize(_window, &width, &height);
			glViewport(0, 0, width, height);
			glClearColor(0.2, 0.2, 0.2, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			_renderer->ProcessRequests(_window);
			glfwSwapBuffers(_window);
			glfwPollEvents();
		}
		return true;
	}

	bool Application::initEngine()
	{
		LOG_INIT_SYS_BEGIN
		if (!initGLFW())		{ return false; }
		if (!initGLEW())		{ return false; }
		if (!initRenderer())	{ return false; }
		LOG_INIT_SYS_END
		return true;
	}

	bool Application::initGLFW()
	{
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

		obtainWindowContext();
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwSwapInterval(1);

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
			auto IvyEngine = (Application*)glfwGetWindowUserPointer(window);
			auto e = EventWindowClosed();
			_Ivy::EventDispatcher::Fire(e);
			IvyEngine->_active = false; });

		glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused) {
			if (focused) {
				auto e = EventWindowFocused();
				_Ivy::EventDispatcher::Fire(e); }
			else {
				auto e = EventWindowLostFocus();
				_Ivy::EventDispatcher::Fire(e); } });

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
			auto e = EventWindowResized(width, height);
			_Ivy::EventDispatcher::Fire(e); });

		glfwSetWindowPosCallback(_window, [](GLFWwindow* window, int xPos, int yPos) {
			auto e = EventWindowMoved(xPos, yPos);
			_Ivy::EventDispatcher::Fire(e); });

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_RELEASE) {
				auto e = EventKeyReleased(key, mods);
				_Ivy::EventDispatcher::Fire(e); }
			else {
				auto e = EventKeyPressed(key, mods, action == GLFW_REPEAT);
				_Ivy::EventDispatcher::Fire(e); } });

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_RELEASE) {
				auto e = EventMouseButtonReleased(button, mods);
				_Ivy::EventDispatcher::Fire(e); }
			else {
				auto e = EventMouseButtonPressed(button, mods);
				_Ivy::EventDispatcher::Fire(e); } });

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
			auto e = EventMouseMoved(xPos, yPos);
			_Ivy::EventDispatcher::Fire(e); });

		glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			auto e = EventMouseScrolled(xOffset, yOffset);
			_Ivy::EventDispatcher::Fire(e); });

		return true;
	}

	bool Application::initGLEW()
	{
		LOG_INIT_SYS_HEADER("OpenGL");
		
		obtainWindowContext();
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

	bool Application::initRenderer()
	{
		_renderer = new _Ivy::Renderer();
		return true;
	}

	void Application::obtainWindowContext()
	{
		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, this);
	}
}