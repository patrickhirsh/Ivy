#include "Core/IvyPCH.h"
#include "Window/Window.h"
#include "Event/Event.h"

namespace _Ivy
{
	Ivy::Ref<Window> Window::Create(
		int width,
		int height,
		std::string name)
	{
		// create window
		auto window = Ivy::Ref<Window>(new Window());
		window->_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
		if (!window->_window)
		{
			LOG_ERROR("GLFW failed to create a new window!");
			return nullptr;
		}

		// init window properties
		window->SetCurrentContext();
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwSwapInterval(1);

		// init GLEW
		if (glewInit() != GLEW_OK)
		{
			LOG_ERROR("GLEW failed to initialize!");
			return nullptr;
		}

		// bind window events
		glfwSetWindowCloseCallback(window->_window, [](GLFWwindow* window) {
			auto thisWindow = (Window*)glfwGetWindowUserPointer(window);
			auto e = Ivy::EventWindowClosed();
			_Ivy::EventDispatcher::Fire(e);
			thisWindow->_active = false; });

		glfwSetWindowFocusCallback(window->_window, [](GLFWwindow* window, int focused) {
			if (focused) {
				auto e = Ivy::EventWindowFocused();
				_Ivy::EventDispatcher::Fire(e); }
			else {
				auto e = Ivy::EventWindowLostFocus();
				_Ivy::EventDispatcher::Fire(e); } });

		glfwSetWindowSizeCallback(window->_window, [](GLFWwindow* window, int width, int height) {
			auto e = Ivy::EventWindowResized(width, height);
			_Ivy::EventDispatcher::Fire(e); });

		glfwSetWindowPosCallback(window->_window, [](GLFWwindow* window, int xPos, int yPos) {
			auto e = Ivy::EventWindowMoved(xPos, yPos);
			_Ivy::EventDispatcher::Fire(e); });

		glfwSetKeyCallback(window->_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			if (action == GLFW_RELEASE) {
				auto e = Ivy::EventKeyReleased(key, mods);
				_Ivy::EventDispatcher::Fire(e); }
			else {
				auto e = Ivy::EventKeyPressed(key, mods, action == GLFW_REPEAT);
				_Ivy::EventDispatcher::Fire(e); } });

		glfwSetMouseButtonCallback(window->_window, [](GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_RELEASE) {
				auto e = Ivy::EventMouseButtonReleased(button, mods);
				_Ivy::EventDispatcher::Fire(e); }
			else {
				auto e = Ivy::EventMouseButtonPressed(button, mods);
				_Ivy::EventDispatcher::Fire(e); } });

		glfwSetCursorPosCallback(window->_window, [](GLFWwindow* window, double xPos, double yPos) {
			auto e = Ivy::EventMouseMoved(xPos, yPos);
			_Ivy::EventDispatcher::Fire(e); });

		glfwSetScrollCallback(window->_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			auto e = Ivy::EventMouseScrolled(xOffset, yOffset);
			_Ivy::EventDispatcher::Fire(e); });

		window->_active = true;
		return window;
	}

	void Window::BeginFrame()
	{
		GLint width, height;
		SetCurrentContext();
		glfwGetWindowSize(_window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(0.2, 0.2, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::EndFrame()
	{
		SetCurrentContext();
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	void Window::SetCurrentContext()
	{
		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, this);
	}

	std::tuple<GLuint, GLuint> Window::GetDimensions()
	{
		GLint width, height;
		SetCurrentContext();
		glfwGetWindowSize(_window, &width, &height);
		return std::tuple<GLuint, GLuint>(width, height);
	}
}